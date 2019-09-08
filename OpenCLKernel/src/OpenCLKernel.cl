float get_iterations(int i_x, int i_y, int i_width, int i_height, int i_max_iterations)
	{
	float cx = 3.5 * i_x / i_width - 2.5;
	float cy = 2.0 * i_y / i_height - 1.0;
	float zx = 0;
	float zy = 0;
	float iter = 0;
	while (zx * zx + zy * zy <= 4 && iter < i_max_iterations)
		{
		float tempzx = zx * zx - zy * zy + cx;
		zy = 2 * zx * zy + cy;
		zx = tempzx;
		iter+=1.0f;
		}
	return iter;
	}

int get_index(int i_x, int i_y, int i_width)
	{
	return i_y * i_width * 4 + i_x * 4;
	}

kernel void mandelbrot_set(int i_max_iterations, global uchar* o_picture)
	{
	int x = get_global_id(0);
	int y = get_global_id(1);
	int width = get_global_size(0);
	int height = get_global_size(1);
	int pixel_coords = get_index(x,y,width);
	float iter = get_iterations(x,y,width,height,i_max_iterations);
	int r = 0;
	if(iter < 34) 
		r = iter * 255 / 33;
	o_picture[pixel_coords + 0] = r;
	o_picture[pixel_coords + 1] = 0;
	o_picture[pixel_coords + 2] = 0;
	o_picture[pixel_coords + 3] = 255;
	}