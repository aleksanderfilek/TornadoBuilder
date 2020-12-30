#include"TE.h"

#include<stdio.h>


int main(){

    if(TE_init_game("Title", 1280, 720) == -1)
        return -1;

    // TE_State *start_state = NULL;
    // if((start_state = TE_create_state(start, update, draw, s_exit)) == NULL)
    //     return -1;

    // TE_start_game(start_state);

    TE_close_game();

    return 0;
}