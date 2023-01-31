    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <time.h>
    #include <math.h>
    #include <assert.h>

    /*
    [
    0 1 2 
    3 4 5
    6 7 8
    ]
    */

    // 0  denotes not filled
    // 1 denotes X
    // 2 denotes O

    struct node {
        // used to calculate uct
        int plays;
        float weight;
        // used to calculate wtf is happening
        int move;
        // used to calculate wtf can happen
        int xmove;
        int *legalmoves; // tracks moves that are possible from the position,
                        // and the nodes which have not been made
        int maxn;
        // used to calculate what has happened 
        int nextn;
        struct node *nextmoves[9];
        // used to understand how it got this bad
        struct node *parent;


    }*root;

    // checks the state of a 3 x 3 game
    int game_state_checker(int *board)
    {
        if ((board[0] == 1) && (board[1] == 1) && (board[2] == 1))
        {
            return 1;
        }
        if ((board[3] == 1) && (board[4] == 1) && (board[5] == 1))
        {
            return 1;
        }
        if ((board[6] == 1) && (board[7] == 1) && (board[8] == 1))
        {
            return 1;
        }
        if ((board[0] == 1) && (board[3] == 1) && (board[6] == 1))
        {
            return 1;
        }
        if ((board[1] == 1) && (board[4] == 1) && (board[7] == 1))
        {
            return 1;
        }
        if ((board[2] == 1) && (board[5] == 1) && (board[8] == 1))
        {
            return 1;
        }
        if ((board[0] == 1) && (board[4] == 1) && (board[8] == 1))
        {
            return 1;
        }
        if ((board[2] == 1) && (board[4] == 1) && (board[6] == 1))
        {
            return 1;
        }
        if ((board[0] == 2) && (board[1] == 2) && (board[2] == 2))
        {
            return 2;
        }
        if ((board[3] == 2) && (board[4] == 2) && (board[5] == 2))
        {
            return 2;
        }
        if ((board[6] == 2) && (board[7] == 2) && (board[8] == 2))
        {
            return 2;
        }
        if ((board[0] == 2) && (board[3] == 2) && (board[6] == 2))
        {
            return 2;
        }
        if ((board[1] == 2) && (board[4] == 2) && (board[7] == 2))
        {
            return 2;
        }
        if ((board[2] == 2) && (board[5] == 2) && (board[8] == 2))
        {
            return 2;
        }
        if ((board[0] == 2) && (board[4] == 2) && (board[8] == 2))
        {
            return 2;
        }
        if ((board[2] == 2) && (board[4] == 2) && (board[6] == 2))
        {
            return 2;
        }
        for (int i = 0; i < 9; i++)
        {
            if (board[i] == 0)
            {
                return 0;
            }
        }
        return 3;
    // returns 0 if game has not ended

    // returns 1 if X wins

    // returns 2 if O wins

    // returns 3 if tied
    }

    // function to print the board
    void print_board(int *board)
    {
        for (int i = 0; i < 9; i++)
        {
            if (board[i] == 0)
            {
                printf(" ");
            }
            if (board[i] == 1)
            {
                printf("X");
            }
            if (board[i] == 2)
            {
                printf("O");
            }
            if ((i + 1) % 3 == 0)
            {
                printf("\n");
            }
            else
            {
                printf("|");
            }
        }
    }

    // if legal move, lm then 1, else 0

    // finds legal moves in 9 * 9 board
    int *findlm(int *calcboard)
    {
        int *lm = malloc(9 * sizeof(int));
        for (int i = 0; i < 9; i++)
        {
            if (calcboard[i] == 0)
            {
                lm[i] = 1;
            }
            else
            {
                lm[i] = 0;
            }
        }
        return lm;
    }

    // prints the weights of the MCT
    void printMCT(struct node* root)
    {
        if (root->nextn == 0)
        {
            printf("%f ", root->weight);
        }
        else
        {
            for (int i = 0; i < root->nextn; i++)
            {
                printMCT((root->nextmoves)[i]);
            }
        }
    }

    // note: in selecting, calcboard should also be updated otherwise it is stupid
    struct node *sel(struct node *root, int *calcboard)
    {
        struct node *node = root;
        if ((node->nextn) < (node->maxn))
        {
            return node;
        }
        else 
        {
            int bestchild = 0;
            float bestuct = 0;
            for (int i = 0; i < (node->nextn); i++)
            {
                struct node *child = (node->nextmoves)[i];
                if (child->plays == 0)
                {
                    printf("children not played");
                }

                float range = 2 * sqrt((logf(node->plays)) / (child->plays));

                float uct = ((child->weight) + range);

                if (uct >= bestuct)
                {
                    bestchild = i;
                    bestuct = uct;
                }
            }
            // play child's move on calcboard here then go look further
            
            struct node *kid = (node->nextmoves)[bestchild];

            if (node -> xmove == 1)
            {
                calcboard[kid -> move] = 1;
            }
            else // if (node -> xmove == 0)
            {
                calcboard[kid->move] = 2;
            }

            return sel(kid, calcboard);
            // calc utc
        }
    }
    struct node *expand(struct node *leaf, int *calcboard)
    {   // randomise over the different legal moves

        int numlegal = (9 - (leaf->nextn));
        

        // given is the set of legal moves as 1

        int m = (rand()%numlegal);

        int counter = m;

        for (int i = 0; i < 9; i++)
        {
            if (leaf->legalmoves[i] == 1)
            {
                counter --;
            }
            if (counter == 0)
            {
                leaf->legalmoves[i] = 0;
            }
        }

        // finally move is set to m


        struct node *newMove = (struct node *)malloc(sizeof(struct node));
        (leaf -> nextmoves)[(leaf->nextn)] = newMove; // array stores addresses of moves that come from this tree
        leaf -> nextn ++; // increments 
    
        newMove ->plays = 0;
        newMove -> weight = 0; // set the weight of the new node to 0. This will change with simulation
        newMove -> move = m;  // notes what move has been made
        if (leaf -> xmove == 0) // sets whether it is x's move or not
        {
            newMove -> xmove = 1;
        }
        else
        {
            newMove -> xmove = 0; 
        }
        newMove -> legalmoves = findlm(calcboard);
        newMove -> nextn = 0;
        newMove -> maxn = ((leaf->maxn) - 1);
        for (int i = 0; i < 9; i++)
        {
            newMove->nextmoves[i] = (struct node *)malloc(sizeof(struct node));
        } // is it sizeof(struct node *) here or sizeof(struct node) ?
        // update calcboard
        if (leaf->xmove == 1)
        {
            calcboard[newMove->move] = 1;
        }
        else
        {
            calcboard[newMove->move] = 2; // if xmove == 0
        }
        newMove->parent = leaf;


        return newMove;


        // assume that calc board has been updated till this point in select()
        // check whose move it is

        // play a random move which has not yet been played
    }

    void simulate(struct node *leaf, int *calcboard) // assume that calcboard has moves played until this point correctly
    {
        // play random moves from the given move all on calcboard until game end is reached
        int xmove = leaf->xmove;

        int possmoves = (leaf->maxn);

        while (game_state_checker(calcboard) == 0)
        {
            // given is the set of legal moves as 1
            int counter = rand()%possmoves; // if possmoves = 0, game (thus loop) will have ended
    // issue is that moves are being played on top of other moves, but possmoves is being decremeted
            for (int i = 0; i < 9; i++)
            {
                if (calcboard[i]== 0)
                {
                    counter --;
                    if (counter <= 0)
                    {   
                        if (xmove == 1)
                        {
                            calcboard[i] = 1;
                            xmove = 0;
                        }
                        else // if xmove = 0
                        {
                            calcboard[i] = 2;
                            xmove = 1;
                        }
                        break;
                    }
                
                }
            }
            possmoves --;
        }

        leaf->plays = 1;

        // when game end is reached, based on tie, win, loss, update weightages.
        if ((game_state_checker(calcboard) == 1) && (leaf->xmove == 0))
        {
            leaf->weight = 1;
            return;
        }
        else if ((game_state_checker(calcboard) == 1) && (leaf->xmove == 1))
        {
            leaf->weight = (-1);
            return;
        }
        else if ((game_state_checker(calcboard) == 2) && (leaf->xmove == 0))
        {
            leaf->weight = (-1);
            return;
        }
        else if ((game_state_checker(calcboard) == 2) && (leaf->xmove == 1))
        {
            leaf->weight = 1;
            return;
        }
        else if ((game_state_checker(calcboard) == 3))
        {
            leaf->weight = 0;
            return;
        }
    }

    void *backpropogate(struct node *leaf)
    {
        struct node *calcnode = leaf;

        int adder;

        while (calcnode->parent != NULL)
        {
            calcnode = calcnode->parent;
            adder = 0;
            for (int i = 0; i < (calcnode->nextn); i++)
            {
                adder += (calcnode->nextmoves[i]->weight);
            }
            calcnode->weight = (adder / (calcnode->nextn));
            calcnode->plays += 1;
        }
    }

    // function to initialise startboard and calcboard

    // function to create the root because I'm neat freak
    void createMCT(struct node *root, int *calcboard)
    {
        // initialise the root
        root-> plays = 0;
        root -> weight = 0; 
        root -> xmove = 1;
        root -> move = -1;
        root -> legalmoves = findlm(calcboard);
        root -> nextn = 0;
        root -> maxn = 9;
        root -> parent = NULL;

        for (int i = 0; i < 9; i++)
        {
            root->nextmoves[i] = (struct node *)malloc(sizeof(struct node));
        } // is it sizeof(struct node *) here or sizeof(struct node) ?
    }

    // ideally, place to store this information to precompute, but that comes much later






    // // if machine starts
    //     // play the first move based on which has best weightage

    //     // change startboard to move machine played and then get human move

    //     // run mcts again
        
    // }

    int main()
    {
        srand(time(0));
        
        // initialise the board state from which game must be played

        // int *startboard = malloc(9 * sizeof(int));
        int startboard[9] = {1, 2, 0, 
                            0, 0, 0,
                            0, 0, 0};

        // initialise the board used for calculation
        int *calcboard = malloc(9 * sizeof(int));
        for (int i = 0; i < 9; i++)
        {
            calcboard[i] = startboard[i];
        }

        struct node *root = (struct node *)malloc(sizeof(struct node));
        createMCT(root, calcboard);

        struct node *currentnode;

        for (int q = 0; q < 100000; q++)
        {
            printf("this is node %d\n", q);
            currentnode = sel(root, calcboard);
                print_board(calcboard);
                printf("\n");
            currentnode = expand(currentnode, calcboard);

            simulate(currentnode, calcboard);

            backpropogate(currentnode);

                print_board(calcboard);
                printf("plays are %d\n", currentnode->plays);
                printf("weight is %f\n", currentnode->weight);
                printf("move is %d\n", currentnode->move);
                printf("xmove is %d\n", currentnode->xmove);
                printf("it has %d children\n", currentnode->nextn);
                printf("it can have upto %d children\n", currentnode->maxn);
                printf("\n");
            for (int i = 0; i < 9; i++)
            {
                calcboard[i] = startboard[i];
            }
        }
        printf("Tree calculated\n");

        // function that gets best startboard

        struct node *bestmove = (root->nextmoves)[0];

        for (int i = 0; i < (root->nextn); i++)
        {
            struct node *now = (root->nextmoves)[i];
            printf("%f ", (now->weight));
            if ((now->weight) >= (bestmove -> weight))
            {
                bestmove = (root->nextmoves)[i];
            }
        }
        printf("%d ", (bestmove->move));



        return 0;
    }

    // expand the tree till game_end_checker == true

    // main function
        // notes possible options
        // randomises over those options
        // expands the tree down 
        // if win then increase path strength by 1
        // if loss then decrease path by 1
        // if tie then dont do anything




/*  the mega tic tac toe board. ayyo rama.
    X|O|O | X|O|X | X|O|X 
    O|X|X | O|X|X | O|X|X 
    O|X|O | O|X|O | O|O|X 
    -------|-------|-------
    X|O|O | X|O|X | X|O|X 
    O|X|X | O|X|X | O|X|X 
    O|X|O | O|X|O | O|O|X 
    -------|-------|-------
    X|O|O | X|O|X | X|O|X 
    O|X|X | O|X|X | O|X|X 
    O|X|O | O|X|O | O|O|X 



    | |  |  | |  |  | |  
    | |  |  | |  |  | |  
    | |  |  | |  |  | |  
    -------|-------|-------
    | |  |  | |  |  | |  
    | |  |  | |  |  | |  
    | |  |  | |  |  | |  
    -------|-------|-------
    | |  |  | |  |  | |  
    | |  |  | |  |  | |  
    | |  |  | |  |  | |  
*/