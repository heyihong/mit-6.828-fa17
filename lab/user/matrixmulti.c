// Lab 4 Challenge
// Concurrent matrix multiplication 

#include <inc/lib.h>

#define POS_WEST 0
#define POS_SOUTH 1
#define POS_CENTER 2
#define POS_EAST 3
#define POS_NORTH 4
#define POS_NONE 5

#define NUM_ROWS 3
#define NUM_COLS 3

#define DIR_UP_BIT 1
#define DIR_LEFT_BIT 2

envid_t root_envid;
envid_t envids[NUM_ROWS + 2][NUM_COLS + 2];
int row_vec[NUM_ROWS];
int col_vec[NUM_COLS];
int matrix[NUM_ROWS][NUM_COLS];


void east_proc() {
    envid_t envid;
    // Only consume value, do nothing
    ipc_recv(&envid, 0, 0);
}

void south_proc(int col) {
    int result;
    result = ipc_recv(NULL, 0, 0);
    ipc_send(root_envid, result, 0, 0);
}

void center_proc(int row, int col) {
    envid_t envid;
    int i, val, result;
    i = DIR_UP_BIT | DIR_LEFT_BIT;
    result = 0;
    while (i > 0) {
        val = ipc_recv(&envid, 0, 0); 
        if (envid == envids[row - 1][col] && (i & DIR_UP_BIT)) {
            result += val; 
            i ^= DIR_UP_BIT;
        } else if (envid == envids[row][col - 1] && (i & DIR_LEFT_BIT)) {
            result += val * matrix[row - 1][col - 1]; 
            ipc_send(envids[row][col + 1], val, 0, 0);
            i ^= DIR_LEFT_BIT;
        } else {
            panic("unexpected envid: %d", envid);
        }
    }
    ipc_send(envids[row + 1][col], result, 0, 0);
}

void west_proc(int row) {
    // wait for start signal
    ipc_recv(NULL, 0, 0);
    ipc_send(envids[row][1], row_vec[row - 1], 0, 0);
}

void north_proc(int col) {
    // wait for start signal
    ipc_recv(NULL, 0, 0);
    ipc_send(envids[1][col], 0, 0, 0);
}

int get_pos(int row, int col) {
    if (row < 0 || row > NUM_ROWS + 1 || col < 0 || col > NUM_COLS + 1) {
        return POS_NONE;
    }
    if ((row == 0 || row == NUM_ROWS + 1) && (col == 0 || col == NUM_COLS + 1)) {
        return POS_NONE;
    }
    if (row == 0) {
        return POS_NORTH;
    }
    if (row == NUM_ROWS + 1) {
        return POS_SOUTH;
    }
    if (col == 0) {
        return POS_WEST;
    }
    if (col == NUM_COLS + 1) {
        return POS_EAST;
    }
    return POS_CENTER;
}

void umain(int argc, char **argv)
{
    int i, j, id, pos, val, envid;
    root_envid = sys_getenvid();
    for (i = 0; i != NUM_ROWS; i++) {
        row_vec[i] = 1;
    }
    for (i = 0; i != NUM_ROWS; i++) {
        for (j = 0; j != NUM_COLS; j++) {
            matrix[i][j] = i * NUM_COLS + j;
        }
    }
    for (i = NUM_ROWS + 1; i >= 0; i--) {
        for (j = NUM_COLS + 1; j >= 0; j--) {
            pos = get_pos(i, j);
            if (pos == POS_NONE) {
                continue;
            }
            if ((id = fork()) < 0) {
                panic("fork: %e", id);
            }
            if (id == 0) {
                switch (pos) {
                    case POS_WEST:
                        west_proc(i);
                        return;
                    case POS_EAST:
                        east_proc();
                        return;
                    case POS_NORTH:
                        north_proc(j);
                        return;
                    case POS_SOUTH:
                        south_proc(j);
                        return;
                    case POS_CENTER:
                        envids[i][j - 1] = ipc_recv(&envid, 0, 0);
                        envids[i - 1][j] = ipc_recv(&envid, 0, 0);
                        center_proc(i, j);
                        return;
                    default:
                        panic("unexpected pos: %d", pos);
                }
            } else {
                envids[i][j] = id;
                if (get_pos(i, j + 1) == POS_CENTER) {
                    ipc_send(envids[i][j + 1], id, 0, 0);
                }
                if (get_pos(i + 1, j) == POS_CENTER) {
                    ipc_send(envids[i + 1][j], id, 0, 0);
                }
            }
        }
    }
    // start west processes
    for (i = 1; i <= NUM_ROWS; i++) {
        ipc_send(envids[i][0], 0, 0, 0);
    }
    // start north processes
    for (i = 1; i <= NUM_COLS; i++) {
        ipc_send(envids[0][i], 0, 0, 0);
    }
    // receive calculate result
    for (i = 0; i != NUM_COLS; i++) {
        val = ipc_recv(&envid, 0, 0);        
        for (j = 1; j <= NUM_COLS; j++) {
            if (envids[NUM_ROWS + 1][j] == envid) {
                col_vec[j - 1] = val;
                break;
            }
        }
        if (j > NUM_COLS) {
            panic("unexpected envid: %d", envid);
        }
    }
    for (i = 0; i != NUM_COLS; i++) {
        if (i == NUM_COLS - 1) {
            cprintf("%d\n", col_vec[i]);
        } else {
            cprintf("%d ", col_vec[i]);
        }
    }
}