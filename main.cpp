#include "Outils.hpp"
#include "Couleur.hpp"
#include "Liste_touche.hpp"
#include "forme/Sphere.hpp"
#include "Camera.hpp"
#include "materiel/Materiel.hpp"
#include "materiel/MaterielDiffuse.hpp"
#include "materiel/MaterielRefract.hpp"
#include "materiel/MaterielSpeculaire.hpp"
#include <iostream>
#include "forme/Rectangle.hpp"
#include "Box.hpp"
#include "Rotation.hpp"
#include "Deplacement.hpp"
#include "Pdf.hpp"
#include <thread>
#include <map>
#include <iterator>

static std::mutex m;

// Calcul la couleur du rayon (on définit la couleur par nous-même)
// Un gradient de bleu à blanc en fonction de la coordonnée Y du rayon
Color rayCouleur(const Ray& r, const Color& fondEcran, const Touche& world, shared_ptr<Liste_touche> &lumiere, int profond) {
    touche_enreg enreg;

    if (profond <= 0)
        return {0, 0, 0};

    // S'il y a une intersection entre le rayon et l'objet, on rend la couleur d'objet
    // Sinon on rend la couleur de fond d'écran
    if (!world.touche(r, 0.001, infini, enreg))
        return fondEcran;

    diffuse_enreg dEnreg;
    Color emise = enreg.mate_ptr->emis(r, enreg, enreg.u, enreg.v, enreg.p);

    if (!enreg.mate_ptr->diffuser(r, enreg, dEnreg))
        return emise;

    if (dEnreg.isSpeculaire) {
        return dEnreg.atenuation * rayCouleur(dEnreg.raySpeculaire, fondEcran, world, lumiere, profond-1);
    }

    auto lumiere_pdf = make_shared<Touche_pdf>(lumiere, enreg.p);
    Melange_pdf p(lumiere_pdf, dEnreg.ptr_pdf);
    Ray diffuse = Ray(enreg.p, p.generateur());
    double pdf_val = p.valeur(diffuse.direction());

    return emise + dEnreg.atenuation * enreg.mate_ptr->diffuser_pdf(r, enreg, diffuse) * rayCouleur(diffuse, fondEcran, world, lumiere, profond - 1) / pdf_val;
}

Liste_touche cornellBox() {
    Liste_touche objects;

    auto violet   = make_shared<MaterielDiffuse>(Color(0.56, 0.42, 0.61));
    auto white = make_shared<MaterielDiffuse>(Color(0.73, 0.73, 0.73));
    auto ros = make_shared<MaterielDiffuse>(Color(0.85, 0.56, 0.65));
    auto bleu = make_shared<MaterielDiffuse>(Color(0.11, 0.79, 0.86));
    auto light = make_shared<LumiereDiffuse>(Color(15, 15, 15));

    // cornellBox
    objects.add(make_shared<RectangleYZ>(0, 555, 0, 555, 555, ros));
    objects.add(make_shared<RectangleYZ>(0, 555, 0, 555, 0, violet));
    objects.add(make_shared<Face_retourne>(make_shared<RectangleXZ>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<RectangleXZ>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<RectangleXZ>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<RectangleXY>(0, 555, 0, 555, 555, bleu));

    shared_ptr<Materiel> aluminum = make_shared<MaterielSpeculaire>(Color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<Touche> box1 = make_shared<Box>(Point3D(0, 0, 0), Point3D(165, 330, 165), aluminum);
    box1 = make_shared<Rotation>(box1, 15);
    box1 = make_shared<Deplacement>(box1, Vecteur3D(265,0,295));
    objects.add(box1);

    shared_ptr<Touche> box2 = make_shared<Box>(Point3D(0,0,0), Point3D(165,165,165), white);
    box2 = make_shared<Rotation>(box2, -18);
    box2 = make_shared<Deplacement>(box2, Vecteur3D(130,0,65));
    objects.add(box2);

    auto verre = make_shared<MaterielRefract>(1.5);
    objects.add(make_shared<Sphere>(Point3D(190,265,190), 90, verre));

    return objects;
}

void calcul(int i, int j, int exemples, Color fondEcran, const Liste_touche& world, int profondMax, Camera cam, int image_width, int image_height, shared_ptr<Liste_touche> lumiere, std::map<int, Color> &couleurs){
    Color pixelCouleur(0, 0, 0);
    for (int s = 0; s < exemples; ++s) {
        double u = (i + random_double()) / (image_width - 1);
        double v = (j + random_double()) / (image_height - 1);
        Ray r = cam.get_ray(u, v);
        pixelCouleur = pixelCouleur + rayCouleur(r, fondEcran, world, lumiere, profondMax);
    }
    Color c = returnCouleur(pixelCouleur, exemples);
    m.lock();
    couleurs.insert(std::pair<int, Color>((image_height-j)*image_width+i, c));
    m.unlock();
}

// On scanne les pixels par ligne de gauche à droite et scanne les lignes de haut en bas
void calculCouleurParallel(int partition, int widthParThread, int exemples, Color fondEcran, const Liste_touche& world, int profondMax, Camera cam, int image_width, int image_height, const shared_ptr<Liste_touche>& lumiere, std::map<int, Color> &couleurs, int nbthread){
    for (int j = image_height-1; j >= 0; --j) {
        if (partition < nbthread-1) {
            for (int i = partition * widthParThread; i < (partition + 1) * widthParThread; i++) {
                calcul(i, j, exemples, fondEcran, world, profondMax, cam, image_width, image_height, lumiere, couleurs);
            }
        }
        if (partition == nbthread-1) {
            for (int i = partition*widthParThread; i<image_width; i++) {
                calcul(i, j, exemples, fondEcran, world, profondMax, cam, image_width, image_height, lumiere, couleurs);
            }
        }
    }
}


int main() {
    
    // Image
    const double aspect_ratio = 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int exemplesParPixel = 100;
    const int profondMax = 50;
    Color fondEcran = Color(0,0,0);
    Point3D lookFrom = Point3D(278, 278, -800);
    Point3D lookAt = Point3D(278, 278, 0);
    Point3D vueEnHaut = Point3D(0, 1, 0);
    double champVisionVert = 40.0;

    // World
    Liste_touche world = cornellBox();
    //shared_ptr<Touche> lumiere = make_shared<RectangleXZ>(213, 343, 227, 332, 554, shared_ptr<Materiel>());
    auto lumiere = make_shared<Liste_touche>();
    lumiere->add(make_shared<RectangleXZ>(213, 343, 227, 332, 554, shared_ptr<Materiel>()));
    lumiere->add(make_shared<Sphere>(Point3D(190,265,190),90, shared_ptr<Materiel>()));

    Camera cam(lookFrom, lookAt, vueEnHaut, champVisionVert, aspect_ratio);

    // Rendu
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    auto start_time = std::chrono::steady_clock::now();

    // calcul en parallel
    int nbThread = (int)std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::map<int, Vecteur3D> couleurs;

    int widthParThread = image_width/nbThread;
    for(int t=0; t<nbThread; t++){
        threads.emplace_back(calculCouleurParallel, t, widthParThread, exemplesParPixel, fondEcran, world, profondMax, cam, image_width, image_height, lumiere, std::ref(couleurs), nbThread);
    }

    for (auto & thread : threads){
        thread.join();
    }

    std::map<int, Color>::iterator iter;
    for(iter = couleurs.begin(); iter != couleurs.end(); ++iter){
        writeCouleur_out(std::cout, iter->second);
    }

    auto end_time = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cerr << "\nFini.\n";
    std::cerr << "Temps de calcule : " << ms << " ms." << std::endl;
}
