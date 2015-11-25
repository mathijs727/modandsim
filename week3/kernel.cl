__constant int2 ci[] = {
	(int2)(0, 0),
	(int2)(0, -1),
	(int2)(0, 1),
	(int2)(-1, 0),
	(int2)(-1, -1),
	(int2)(-1, 1),
	(int2)(1, 0),
	(int2)(1, -1),
	(int2)(1, 1)
};
__const float omega = 1.0;
__constant float cs = xxx;

void stream(__global const float* data)
{
	int width = get_global_size(0);
	int height = get_global_size(1);

	int localX = get_local_id(0);
	int localY = get_local_id(1);

	int globalX = get_global_id(0);
	int globalY = get_global_id(1);
}

void collide(__global const float* data)
{

}

__kernel void sum(__global const float *a_g, __global const float *b_g, __global float *res_g) {
	int gid = get_global_id(0);
	res_g[gid] = a_g[gid] + b_g[gid];
}