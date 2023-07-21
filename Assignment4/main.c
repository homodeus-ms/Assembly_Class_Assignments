#include "transformer.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define ASSERT_MAT_EQUALS(expected, actual) assert(memcmp(expected, actual, sizeof(mat4_t)) == 0)
#define ASSERT_VEC_EQUALS(expected, actual) assert(memcmp(expected, actual, sizeof(vec4_t)) == 0)

int main(void)
{
    enum { LENGTH = 2 };
    vec4_t arr[LENGTH];
    read_points(arr, LENGTH);

    for (size_t i = 0; i < LENGTH; ++i) {
        printf("%f %f %f %f\n", arr[i].x, arr[i].y, arr[i].z, arr[i].w);
    }
#if 0
    {
        mat4_t expected = {
            { 1.f, 5.f,  9.f, 13.f },
            { 2.f, 6.f, 10.f, 14.f },
            { 3.f, 7.f, 11.f, 15.f },
            { 0.f, 0.f,  0.f,  1.f }
        };

        mat4_t mat = {
            {  1.f, 2.f,   3.f, 0.f },
            {  5.f, 6.f,   7.f, 0.f },
            {  9.f, 10.f, 11.f, 0.f },
            { 13.f, 14.f, 15.f, 1.f },
        };

        transpose(&mat);

        ASSERT_MAT_EQUALS(&expected, &mat);
    }

    {
        vec4_t expected = { 125.234001f, 563.234314f, 899.893677f, 1.f };
        vec4_t src = { 123.2340f, 566.2343f, 897.8937f, 1.0f };
        vec4_t dst;
        mat4_t mat = {
            { 1.f, 0.f, 0.f,  2.f },
            { 0.f, 1.f, 0.f, -3.f },
            { 0.f, 0.f, 1.f,  2.f },
            { 0.f, 0.f, 0.f,  1.f }
        };

        transform(&dst, &src, &mat); /* dst: { x: 125.234001f, y: 563.234314f, z: 899.893677f, w: 1.f } */

        ASSERT_VEC_EQUALS(&expected, &dst);
    }

    {
        mat4_t expected = {
            { 2.f,  0.f, 0.f,  0.f },
            { 0.f, 0.5f, 0.f,  0.f },
            { 0.f,  0.f, 1.f,  0.f },
            { 2.f, -3.f, 2.f,  1.f }
        };

        mat4_t m0 = {
            { 2.f, 0.f,  0.f, 0.f },
            { 0.f, 0.5f, 0.f, 0.f },
            { 0.f, 0.f,  1.f, 0.f },
            { 0.f, 0.f,  0.f, 1.f }
        };
        mat4_t m1 = {
            { 1.f, 0.f, 0.f,  2.f },
            { 0.f, 1.f, 0.f, -3.f },
            { 0.f, 0.f, 1.f,  2.f },
            { 0.f, 0.f, 0.f,  1.f }
        };
        mat4_t res;

        concatenate(&res, &m0, &m1);

        ASSERT_MAT_EQUALS(&expected, &res);
    }

    {
        size_t i;
        enum { NUM_POINTS = 3 };

        vec4_t expected[NUM_POINTS] = {
            { 612.458862f, 502.862152f, 566.409790f, 1.0f },
            { 105.402145f, -333.766022f, -167.346207f, 1.0f },
            { -109.100571f, -130.558533f, -702.911804f, 1.0f }
        };

        vec4_t local_points[NUM_POINTS] = {
            { 123.234f, 566.234231f, 897.89375f, 1.f },
            {  87.111f, -589.83745f,   -177.33f, 1.f },
            { 128.1200f,   98.3341f, -676.3423f, 1.f }
        };
        vec4_t world_points[NUM_POINTS];

        const vec3_t scale = { 2.f, 0.5f, 1.f };
        const vec3_t rotation = { -15.f, 30.f, 0.f };
        const vec3_t translation = { 2.f, -3.f, 2.f };

        run(world_points, local_points, NUM_POINTS, &scale, &rotation, &translation);

        for (i = 0; i < NUM_POINTS; ++i) {
            ASSERT_VEC_EQUALS(&expected[i], &world_points[i]);
        }
    }
#endif
    printf("No Assert!");

    return 0;
}
