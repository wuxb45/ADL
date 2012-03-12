#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct __COMPLEX FFT_COMPLEX;


struct __COMPLEX {
	double real;
	double image;
};


void
complex_array_print(int n, FFT_COMPLEX * array)
{
	int i;
	printf("COMPLEX[%d]:\n", n);
	for(i = 0; i < n; i++){
		printf("(%lf,%lfi)", array[i].real, array[i].image);
	}
	printf(";\n");
}

FFT_COMPLEX
complex_add(FFT_COMPLEX * a, FFT_COMPLEX * b)
{
	FFT_COMPLEX r;
	r.real = a->real + b->real;
	r.image = a->image + b->image;
	return r;
}

FFT_COMPLEX
complex_sub(FFT_COMPLEX * a, FFT_COMPLEX * b)
{
	FFT_COMPLEX r;
	r.real = a->real - b->real;
	r.image = a->image - b->image;
	return r;
}

FFT_COMPLEX
complex_mul(FFT_COMPLEX * a, FFT_COMPLEX * b)
{
	FFT_COMPLEX r;
	r.real = a->real * b->real - a->image * b->image;
	r.image = a->real * b->image + a->image * b->real;
	return r;
}

FFT_COMPLEX
get_w(int n, int N)
{
	FFT_COMPLEX t;

	double angel = 2 * M_PI * ((double)n) / ((double)N);

	t.real = cos(angel);
	t.image = -sin(angel);
	printf("w(%d/%d) = (%lf,%lfi)\n", n, N, t.real, t.image);
	return t;
}


void rfft(int n, FFT_COMPLEX * in, FFT_COMPLEX * out)
{
	FFT_COMPLEX * in_t0;
	FFT_COMPLEX * in_t1;

	FFT_COMPLEX * out_t0;
	FFT_COMPLEX * out_t1;

	FFT_COMPLEX z;
	FFT_COMPLEX w;
	FFT_COMPLEX t;
	int i;
	int n2;

	n2 = n >> 1;
	
	if(n == 1){
		*out = *in;
		return;
	}

	complex_array_print(n, in);
	
	w = get_w(1, n);
	z = get_w(0, n);

	in_t0 = (FFT_COMPLEX *)malloc(n2 * sizeof(FFT_COMPLEX));
	in_t1 = (FFT_COMPLEX *)malloc(n2 * sizeof(FFT_COMPLEX));
	out_t0 = (FFT_COMPLEX *)malloc(n2 * sizeof(FFT_COMPLEX));
	out_t1 = (FFT_COMPLEX *)malloc(n2 * sizeof(FFT_COMPLEX));

	for(i = 0; i < n; i++){
		if((i & 1) == 0){
			in_t0[i>>1] = in[i];
		}else{
			in_t1[i>>1] = in[i];
		}
	}
	
	// RFFT(a0,a2,...,an-2, u0,u1,...,u(n/2)-1)
	rfft(n2, in_t0, out_t0);
	// RFFT(a1,a3,...,an-1, v0,v1,...,v(n/2)-1)
	rfft(n2, in_t1, out_t1);

	// z=1


	for(i = 0; i < (n >> 1); i++){

		t = complex_mul(&z, &(out_t1[i]));

		out[i] = complex_add(&(out_t0[i]), &t);

		printf("[%d] : out[%d](%lf,%lfi) = (%lf,%lfi) + ( (%lf,%lfi) * (%lf,%lfi));\n",
				n, i,
				out[i].real, out[i].image,
				out_t0[i].real, out_t0[i].image,
				z.real, z.image,
				out_t1[i].real, out_t1[i].image);

		out[i + n2] = complex_sub(&(out_t0[i]), &t);

		printf("[%d] : out[%d](%lf,%lfi) = (%lf,%lfi) - ( (%lf,%lfi) * (%lf,%lfi));\n",
				n, i+ n2,
				out[i + n2].real, out[i + n2].image,
				out_t0[i].real, out_t0[i].image,
				z.real, z.image,
				out_t1[i].real, out_t1[i].image);

		printf("[%d] : W is (%lf, %lfi)\n", n, w.real, w.image);
		printf("[%d] : Z is (%lf, %lfi)\n", n, z.real, z.image);

		z = complex_mul(&w, &z);
		printf("[%d] : next Z is (%lf, %lfi)\n", n, z.real, z.image);

		//printf("round %d : z=(%lf, %lfi)", i, z.real, z.image);
	}

	complex_array_print(n, out);

}



int
main(int argc, char * argv[])
{
	FFT_COMPLEX test[4];
	FFT_COMPLEX result[4];
	test[0].real = 1;
	test[0].image = 0;
	test[1].real = 2;
	test[1].image = 0;
	test[2].real = -1;
	test[2].image = 0;
	test[3].real = 3;
	test[3].image = 0;

	rfft(4,test,result);
	complex_array_print(4, result);
	return 0;
}

