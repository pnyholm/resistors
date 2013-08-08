/***********************************************************************
 *  State Machines
 *  Ali Wood
 *  MEEn 495R
 ***********************************************************************/
#include <p24F04KA201.h>
#include <stdlib.h>
#include <stdbool.h>

/************  Global Variables and Function Declarations  *************/
// states of Navigation FSM
typedef enum {LOCATE,CENTER,FIND_HOLE,WAIT,ROTATE90,FIND_SNITCH,GET_SNITCH} NavState;
// states of SortShoot FSM
typedef enum {ORGANIZE,WAIT,PREP,SHOOT,FS_WAIT,WIN} SSState;

// signals for cross talk
bool located = 0;
bool centered = 0;
bool holeFound = 0;
bool foundSnitch = 0;
bool organized = 0;
int shootCount;

//  FSM declarations
NavState NavigationFSM(NavState NS);
SSState SortShootFSM(SSState SS);

/************  FSM Implementation  *************/

NavigationFSM(NavState curState){
    NavState nextState;
    switch(curState){
        case LOCATE:
            if(located == 1)
                nextState = CENTER;
            else
                nextState = LOCATE;

        case CENTER:
            if(centered == 1)
                nextState = FIND_HOLE;
            else
                nextState = CENTER;

        case FIND_HOLE:
            if(holeFound == 1)
                nextState = WAIT;
            else
                nextState = FIND_HOLE;

        case WAIT:
            if(shootCount == 6)
                nextState = FIND_SNITCH;
            else if(shootCount == 2 || shootCount == 4)
                nextState = ROTATE90;
            else
                nextState = WAIT;

        case ROTATE90:
            nextState = FIND_HOLE;

        case FIND_SNITCH:
            if(foundSnitch == 1)
                nextState = GET_SNITCH;
            else
                nextState = FIND_SNITCH;

        case GET_SNITCH:
            nextState = CENTER;

        //default:
    }
}

SortShootFSM(SSState curState){
    SSState nextState;
    switch(curState){
        case ORGANIZE:
            if(organized == 1)
                nextState = WAIT;
            else
                nextState = ORGANIZE;

        case WAIT:
            if(holeFound == 1)
                nextState = PREP;
            else
                nextState = WAIT;
        
        case PREP:
            nextState = SHOOT;

        case SHOOT:
            shootCount++;
            if(shootCount == 2 || shootCount == 4)       // you have shot both colored balls
                nextState = WAIT;
            else if(shootCount == 6)    // you have shot all normal balls
                nextState = FS_WAIT;
            else                        // you have one more color ball to shoot
                nextState = SHOOT;

        case FS_WAIT:
            if(foundSnitch == 1)
                nextState = WAIT;
            else
                nextState = FS_WAIT;

        case WIN:;

        default:;

    }

}
