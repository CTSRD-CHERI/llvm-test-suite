/* For copyright information, see olden_v1.0/COPYRIGHT */

/* ========== PROCEDURE TYPES/NUMS ================== */


HANDLE *RandTree(int n, int seed, int node, int level);

void SwapValue(HANDLE *l, HANDLE *r);
void SwapValLeft(HANDLE *l, HANDLE *r, HANDLE *ll, HANDLE *rl, int lval, int rval);
void SwapValRight(HANDLE *l, HANDLE *r, HANDLE *ll, HANDLE *rl, int lval, int rval);
int Bimerge(HANDLE *root, int spr_val, int dir);
int Bisort(HANDLE *root, int spr_val, int dir);
int dealwithargs(int argc, char *argv[]);
