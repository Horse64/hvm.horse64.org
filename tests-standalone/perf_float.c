
#define SPEW3D_OPTION_DISABLE_SDL
#define SPEW3D_IMPLEMENTATION
#include <spew3d.h>

#include <stdint.h>
#include <stdio.h>

int main(int argc, const char** argv) {
    s3dnum_t x = S3D_DBLTONUM(1.01);
    s3dnum_t xstart = x;
    s3dnum_t y = S3D_DBLTONUM(1.01);
    s3dnum_t z = S3D_DBLTONUM(0.9);
    char outputbuf[256];
    int k = 1000000;
    while (k > 0) {
        x = xstart;
        int i = 0;
        while (i < 1000) {
            x = S3D_NUMMULT(x, y);
            i += 1;
        }
        x = S3D_NUMMULT(x, z);
        i = 0;
        while (i < 1000) {
            x = S3D_NUMMULT(x, y);
            i += 1;
        }
        snprintf(outputbuf, sizeof(outputbuf),
            "RESULT FLOAT: %f\n", S3D_NUMTODBL(x));
        if (k == 1)
            printf("%s", outputbuf);
        k -= 1;
    }
}

