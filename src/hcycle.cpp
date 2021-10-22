# include "../include/hcycle.h"

HamiltonianCycle::HamiltonianCycle(int cols, int rows)
    : m_cols(cols), m_rows(rows){

    assert(this->m_cols % 2 == 0 && this->m_rows % 2 == 0);

    this->m_hcycle = new int[this->m_cols * this->m_rows];
    
    this->hcycle_gen_alg();
}

HamiltonianCycle::~HamiltonianCycle(){
    delete[] this->m_hcycle;
}

/***
 * PRIM MAZE GENERATOR
 * 
 */

void HamiltonianCycle::add_pwalls(std::set<Vector2i>& pwalls, int pcell){
    if(pcell % (this->m_cols / 2) != 0){                      // left
        pwalls.insert(Vector2i(pcell - 1, pcell));
    }
    if((pcell + 1) % (this->m_cols / 2) != 0){                // right
        pwalls.insert(Vector2i(pcell, pcell + 1));
    }
    if(pcell >= (this->m_cols / 2)){                          // up
        pwalls.insert(Vector2i(pcell - (this->m_cols / 2), pcell));
    }
    if(pcell < ((this->m_rows / 2)- 1) * (this->m_cols / 2)){ // down
        pwalls.insert(Vector2i(pcell, pcell + (this->m_cols / 2)));
    }
}

void HamiltonianCycle::pmaze_gen_alg(std::vector<int>* pmaze){
    // https://en.wikipedia.org/wiki/Maze_generation_algorithm

    // start with a grid full of walls
    bool* pcells;
    std::set<Vector2i> pwalls;

    pcells = new bool[(this->m_cols / 2) * (this->m_rows / 2)];
    for(int i = 0; i < (this->m_cols / 2) * (this->m_rows / 2); i++){
        pcells[i] = false;
    }

    // pick random cell, set to visited
    int pcell = std::rand() % ((this->m_cols / 2) * (this->m_rows / 2));
    pcells[pcell] = true;

    // add adjacent walls to wall list
    this->add_pwalls(pwalls, pcell);

    // while wall list not empty
    while(!pwalls.empty()){

        // pick random wall from wall list
        auto pwall = pwalls.begin();
        std::advance(pwall, rand() % pwalls.size());

        // if wall has only one visited cell
        if(!pcells[pwall->x] != !pcells[pwall->y]){
            
            // wall is edge, add to maze
            pmaze[pwall->x].push_back(pwall->y);
            pmaze[pwall->y].push_back(pwall->x);

            // set unvisited cell to visited
            pcell = pcells[pwall->x] ? pwall->y : pwall->x;
            pcells[pcell] = true;

            // add adjacent walls to wall list
            this->add_pwalls(pwalls, pcell);
        }

        // remove wall from wall list
        pwalls.erase(pwall);
    }

    delete[] pcells;
}

/***
 * HAMILTONIAN CYCLE
 * 
 */

void HamiltonianCycle::hcycle_gen_alg(){
    std::vector<int>* pmaze = new std::vector<int>[(this->m_cols / 2) * (this->m_rows / 2)];

    this->pmaze_gen_alg(pmaze);

    int hcell = 0, hdir = 0, hcount = 0;

    do{
        this->m_hcycle[hcount] = hcell;
        hcount++;

        int hnext;
        int pcell = ((hcell / this->m_cols) / 2) * (this->m_cols / 2) + ((hcell % this->m_cols) / 2);
        for(int i = 0; i < 3; i++){
            if(i == 0){ hnext = (hdir + 3) % 4; }
            else if(i == 1){ hnext = (hdir + 4) % 4; }
            else if(i == 2){ hnext = (hdir + 5) % 4; }

            if(hnext == 0 && ((hcell / this->m_cols) % 2 == 1 || v_contains(pmaze[pcell], pcell - (this->m_cols / 2)))){       // up
                hcell -= this->m_cols; 
                hdir = 0;
                break;

            }else if(hnext == 1 && (hcell % 2 == 0 || v_contains(pmaze[pcell], pcell + 1))){                                   // right
                hcell += 1; 
                hdir = 1;
                break;

            }else if(hnext == 2 && ((hcell / this->m_cols) % 2 == 0 || v_contains(pmaze[pcell], pcell + (this->m_cols / 2)))){ // down
                hcell += this->m_cols; 
                hdir = 2;
                break;

            }else if(hnext == 3 && (hcell % 2 == 1 || v_contains(pmaze[pcell], pcell - 1))){                                   // left
                hcell -= 1; 
                hdir = 3;
                break;

            }
        }
    }while(hcount < this->m_cols * this->m_rows);

    // this->print_debug(pmaze);
    
    delete[] pmaze;
}

const int* HamiltonianCycle::get_hcycle() const{
    return this->m_hcycle;
}

int HamiltonianCycle::get_next_pos(int pos){
    return this->m_hcycle[(pos + 1) % (this->m_cols * this->m_rows)];
}

/***
 * TEST
 * 
 */

void HamiltonianCycle::print_debug(const std::vector<int>* pmaze){

    // print pmaze
    std::cout << "\nMaze:\n";
    for(int i = 0; i < (this->m_cols / 2); i++){
        std::cout << " _";
    }
    std::cout << "\n";
    for(int i = 0; i < (this->m_rows / 2); i++){
        std::cout << "|";
        for(int j = 0; j < (this->m_cols / 2); j++){
            if(v_contains(pmaze[i * (this->m_cols / 2) + j], (i + 1) * (this->m_cols / 2) + j)){
                std::cout << " "; 
            }else{
                std::cout << "_"; 
            }
            if(v_contains(pmaze[i * (this->m_cols / 2) + j], i * (this->m_cols / 2) + (j + 1))){
                std::cout << " "; 
            }else{
                std::cout << "|"; 
            }
        }
        std::cout << "\n";
    }

    // print hcycle
    std::cout << "\nHamiltonian cycle:\n\n"; 
    for(int i = 0; i < this->m_cols * this->m_rows; i++){
        std::cout << this->m_hcycle[i] << " ";
    }
    std::cout << "\n";
}