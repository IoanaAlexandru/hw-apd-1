             _    ____  ____            _____                      _ 
            / \  |  _ \|  _ \          |_   _|__ _ __ ___   __ _  / |
           / _ \ | |_) | | | |  _____    | |/ _ \ '_ ` _ \ / _` | | |
          / ___ \|  __/| |_| | |_____|   | |  __/ | | | | | (_| | | |
         /_/   \_\_|   |____/            |_|\___|_| |_| |_|\__,_| |_|


  ___          _     _     ___ ___   _      _   
 | _ \__ _ _ _| |_  / |   / __/ __| /_\    /_\  
 |  _/ _` | '_|  _| | |_  \__ \__ \/ _ \  / _ \ 
 |_| \__,_|_|  \__| |_(_) |___/___/_/ \_\/_/ \_\


Structuri folosite:

 - image: conține datele din fișierul PPM/PGM
   * type: tipul imaginii (5 pentru P5/PGM, 6 pentru P6/PPM)
   * width: lățimea imaginii
   * height: înălțimea imaginii
   * maxval: valoarea maximă a unui pixel (<= 255)
   * image: matrice care conține valorile pixelilor ce desciu imaginea
 - thread_func_args: conține argumentele de care are nevoie thread_func
   * in: imaginea inițială
   * out: imaginea obținută după redimensionare\
   * thread_id: id-ul thread-ului


Funcții:

 - alloc_image:  alocă matricea de pixeli dintr-o imagine
 - free_image:   eliberează matricea de pixeli dintr-o imagine
 - resize_bw:    redimensionează imaginea inițială (greyscale) în funcție de
                 resize_factor
 - resize_color: redimensionează imaginea inițială (color) în funcție de
                 resize_factor
 - readInput:    citește datele dintr-un fișier și le memorează într-o structură
                 de tip <image>
 - writeData:    scrie datele dintr-o structură de tip <image> într-un fișier
 - resize:       folosește funcțiile resize_bw și resize_color pentru a redimen-
                 siona o imagine (mai exact, a o micșora de resize_factor ori)
                 folosind mai multe fire de execuție (num_threads).


Algoritm redimensionare:

 Pentru fiecare pixel din noua imagine, se calculează valoarea (pe un singur
canal de culoare în cazul resize_bw, sau pe trei canale în cazul resize_color)
în funcție de cei resize_factor * resize_factor pixeli corespunzători din ima-
ginea originală. În cazul în care resize_factor este diferit de 3, se calculea-
ză media aritmetică a pixelilor din imaginea originală. În cazul în care
resize_factor este 3, se înmulțește fiecare dintre cei 9 pixeli corespunzători
din imaginea originală cu elementul de pe poziția sa din kernel-ul gaussian, iar
rezultatul se împarte la 16 (suma elementelor din kernel-ul gaussian). 


Paralelizare:

 Imaginea este împărțită pe verticală în num_threads, iar fiecare thread calcu-
lează valorile din partea corespunzătoare a imaginii. Această abordare scalează
bine în cazul imaginilor a căror înălțime este mai mare decât lățimea.

 Alternativ, dacă imaginile pe care lucrăm au predominant lățimea mai mare decât
înălțimea (un caz mai puțin întâlnit în practică, în general), imaginea ar putea
fi împărțită pe orizontală, cazul în care primele 2 bucle din funcție ar arăta
așa:

     for (i1 = 0; i1 < out->height; i1++)
        for (j1 = start; j1 < end; j1++)

unde:

     start = thread_id * (out->width) / num_threads
     end = (thread_id + 1) * (out->width) / num_threads

  ___          _     ___     ___             _                 
 | _ \__ _ _ _| |_  |_  )   | _ \___ _ _  __| |___ _ _ ___ _ _ 
 |  _/ _` | '_|  _|  / / _  |   / -_) ' \/ _` / -_) '_/ -_) '_|
 |_| \__,_|_|  \__| /___(_) |_|_\___|_||_\__,_\___|_| \___|_|  

TODO