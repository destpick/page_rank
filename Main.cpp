#include "Page.h"
#include "Graph.h"

///// NOTE REMOVE RANK MOFO
int main(int argc, char* argv[])
{
        /////////////////////////////// VARIABLE DECLARATION ///////////////////
        std::ifstream input_file;
        std::string file_name;

        // Will change to reflect that of the users input
        int walk_count = 0;
        double dampening_ratio = 0.0;
        int thread_num = 0;
        int total_possible_visits = 0;
        double total_time = 0.0;
        Graph* network = new Graph();

        ///////////////////////////// SETTING APPROPRIATE VARIABLES ///////////////
        // If not enough arguements are provided
        if (argc <= 3)
        {
                std::cout << "Not enough arguments provided." << std::endl;
                std::cout << "Expecting in this order:\
                                        \n\t1) Filename to read from.\
                                        \n\t2) Size of walk.\
                                        \n\t3) Dampening ratio.\
                                        \n\t4) Thread count.\
                                        \n Click any button to continue."
                        << std::endl;
                getchar();
                exit(0);
        }

        // If to many arguments are provided
        else if (argc >= 6)
        {
                std::cout << "To many arguments provided." << std::endl;
                std::cout<< "Expecting in this order:\
				                                        \n\t1) Filename to read from.\
                                        \n\t2) Size of walk.\
                                        \n\t3) Dampening ratio.\
                                        \n\t4) Thread count.\
                                        \n Click any button to continue."
                        << std::endl;
                getchar();
                exit(0);
        }

        // Obtain user provided information
        else
        {
                file_name = argv[1];
                walk_count = atoi(argv[2]);
                dampening_ratio = atof(argv[3]);
                thread_num = atoi(argv[4]);
        }

        ///////////////////////////////////// CREATING THE NETWORK (MATRIX) ////////

        // Open stream for reading
        input_file.open(file_name, std::ios::in);
        if (!input_file.is_open())
        {
                perror("Failed to open file: ");
                exit(0);
        }


        // Disable dynamic teams
        omp_set_dynamic(0);

        // Use p threads for all parallel aspects of code
        omp_set_num_threads(thread_num);

        create_network(network, input_file);
		
        // determing the possible number of times an individual page could have beenvisited
        total_possible_visits = walk_count * network->get_num_of_pages();

        //////////////////////// PAGE RANK MOTHER FUCKERS //////////////////////////
///////////
        total_time = omp_get_wtime();
        page_rank(network, walk_count, dampening_ratio);
        total_time = omp_get_wtime() - total_time;
        //////////////////////// FIND MAX FIVE /////////////////////////////////////
////////
        Page** top_five = (Page**)malloc(5 * sizeof(Page*));

        std::cout<<"total amount of time to run: " << total_time << std::endl;
        // Make empty page array
        create_empty_size_five_array(top_five);


        //display_network(network->return_list(), network->get_num_of_pages());

        find_first_five_max(network, top_five, total_possible_visits);

        display_network(top_five, 5);


}
