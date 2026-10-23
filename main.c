#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/debugScreen.h>
#include <stdlib.h>

#define ROSTER_SIZE 8

typedef struct {
    const char *name;
    int strength;
    int speed;
    int technique;
} Wrestler;

static Wrestler roster[ROSTER_SIZE] = {
    {"Rex Steel", 8, 6, 5},
    {"Blaze Carter", 6, 9, 6},
    {"Titan Knox", 9, 4, 6},
    {"Ace Storm", 5, 9, 8},
    {"Mason King", 7, 6, 8},
    {"Riot Vance", 8, 7, 4},
    {"Duke Phoenix", 6, 7, 9},
    {"Shadow Wolf", 7, 8, 7}
};

static void wait_frames(void) {
    sceKernelDelayThread(150000);
}

static void wait_for_release(void) {
    SceCtrlData pad;
    do {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        sceKernelDelayThread(30000);
    } while (pad.buttons != 0);
}

static int choose_wrestler(void) {
    SceCtrlData pad;
    int selected = 0;

    while (1) {
        psvDebugScreenClear(0);
        psvDebugScreenPrintf("================================\n");
        psvDebugScreenPrintf("       RING WARRIORS VITA\n");
        psvDebugScreenPrintf("================================\n\n");
        psvDebugScreenPrintf("Choose your wrestler:\n\n");

        for (int i = 0; i < ROSTER_SIZE; i++) {
            psvDebugScreenPrintf("%s %s\n",
                i == selected ? ">" : " ", roster[i].name);
        }

        psvDebugScreenPrintf("\nLEFT/RIGHT: Select wrestler\n");
        psvDebugScreenPrintf("CROSS: Start match\n");
        psvDebugScreenPrintf("START: Quit\n");

        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons & SCE_CTRL_LEFT) {
            selected = (selected + ROSTER_SIZE - 1) % ROSTER_SIZE;
            wait_for_release();
        } else if (pad.buttons & SCE_CTRL_RIGHT) {
            selected = (selected + 1) % ROSTER_SIZE;
            wait_for_release();
        } else if (pad.buttons & SCE_CTRL_CROSS) {
            wait_for_release();
            return selected;
        } else if (pad.buttons & SCE_CTRL_START) {
            return -1;
        }
        wait_frames();
    }
}

static void play_match(int player_id, int opponent_id) {
    SceCtrlData pad;
    Wrestler *player = &roster[player_id];
    Wrestler *opponent = &roster[opponent_id];
    int player_hp = 100, opponent_hp = 100, stamina = 100;
    int player_score = 0, opponent_score = 0, tick = 0, finished = 0;

    while (!finished) {
        psvDebugScreenClear(0);
        psvDebugScreenPrintf("=========== LIVE MATCH ===========\n\n");
        psvDebugScreenPrintf("YOU: %s\nHP: %d/100  STAMINA: %d/100\n",
                             player->name, player_hp, stamina);
        psvDebugScreenPrintf("\nVS\n\nOPPONENT: %s\nHP: %d/100\n",
                             opponent->name, opponent_hp);
        psvDebugScreenPrintf("\nCROSS: Strike\nSQUARE: Grapple\n");
        psvDebugScreenPrintf("TRIANGLE: Recover stamina\nCIRCLE: Pin attempt\n");
        psvDebugScreenPrintf("START: Leave match\n");

        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons & SCE_CTRL_START) {
            wait_for_release();
            finished = 1;
            continue;
        } else if (pad.buttons & SCE_CTRL_CROSS) {
            if (stamina >= 5) {
                int damage = 5 + player->speed / 2;
                opponent_hp -= damage;
                stamina -= 5;
                player_score += damage;
            }
            wait_for_release();
        } else if (pad.buttons & SCE_CTRL_SQUARE) {
            if (stamina >= 12) {
                int damage = 8 + player->technique;
                opponent_hp -= damage;
                stamina -= 12;
                player_score += damage;
            }
            wait_for_release();
        } else if (pad.buttons & SCE_CTRL_TRIANGLE) {
            stamina += 12;
            if (stamina > 100) stamina = 100;
            wait_for_release();
        } else if (pad.buttons & SCE_CTRL_CIRCLE) {
            if (opponent_hp <= 30) finished = 1;
            wait_for_release();
        }

        if (opponent_hp <= 0) {
            opponent_hp = 0;
            finished = 1;
            continue;
        }

        tick++;
        if (tick % 3 == 0) {
            int damage = 4 + opponent->strength / 2;
            player_hp -= damage;
            if (player_hp < 0) player_hp = 0;
            opponent_score += damage;
        }
        if (stamina < 100 && tick % 2 == 0) stamina += 2;
        if (player_hp <= 0) finished = 1;
        wait_frames();
    }

    psvDebugScreenClear(0);
    psvDebugScreenPrintf("=========== MATCH RESULT ===========\n\n");
    if (player_hp > 0 && (opponent_hp <= 0 || player_score >= opponent_score))
        psvDebugScreenPrintf("YOU WON!\n\n");
    else
        psvDebugScreenPrintf("MATCH OVER\n\n");
    psvDebugScreenPrintf("%s: %d points\n", player->name, player_score);
    psvDebugScreenPrintf("%s: %d points\n", opponent->name, opponent_score);
    psvDebugScreenPrintf("\nCROSS: Return to roster\nSTART: Quit\n");

    while (1) {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons & SCE_CTRL_CROSS) {
            wait_for_release();
            break;
        }
        if (pad.buttons & SCE_CTRL_START) sceKernelExitProcess(0);
        wait_frames();
    }
}

int main(void) {
    SceCtrlData pad;
    psvDebugScreenInit();
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_DIGITAL);

    while (1) {
        int player_id = choose_wrestler();
        if (player_id < 0) break;
        int opponent_id = (player_id + 1 + rand() % (ROSTER_SIZE - 1)) % ROSTER_SIZE;
        play_match(player_id, opponent_id);
    }
    sceKernelExitProcess(0);
    return 0;
}
