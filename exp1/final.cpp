#include<iostream>
#include<bits/stdc++.h>
#include<fstream>
#include<string>
#include<vector>
#include<math.h>

using namespace std;

typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;
typedef int int32_t;
#define PI 3.14159265

#pragma pack(push, 1)

typedef struct BITMAPIMAGE
{
	uint16_t signature;
	uint32_t fsize;
	uint32_t reserve;
	uint32_t offset;

	uint32_t header_size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitwidth;
	uint32_t compression;
	uint32_t imsize;
	uint32_t Xresol;
	uint32_t Yresol;
	uint32_t Ucolors;
	uint32_t Icolors;
	unsigned char *color_table;	
}BITMAP;


typedef struct _BGRA 
{
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Alpha;
}BGRA;

#pragma pack(pop)

class image
{
	public:
	BITMAP A;

	image(){
		
		// cout<<"Const" << endl;
	}

	BITMAP readbmp(char* s)
	{
		int i;
		cout << "Filename" << " " << s << endl;
		FILE* f = fopen(s, "rb");
		FILE* ifile = fopen(s, "rb");
		
		if(f==NULL)
		{
			cout << "Error in Opening File" << endl;
			return A;
		}
		unsigned char fileheader[54];
	    fread(fileheader, sizeof(unsigned char), 54, f);
	    string filestruct = "";
		
	  	for(uint8_t i=0;i<sizeof(fileheader);i++)
	  	{
	  		if(i<2)
	  			filestruct += fileheader[i];
	  		else	break;
	  	}
	  	if(filestruct != "BM")
	  	{
	  		cout << "Incorrect File Format" << endl;
	  		return A;
	  	}
	  	

	  	int width, height, filesize, bitwidth, offset, reserve, headsize, planes, compression, imsize, Xresol, Yresol, Ucolors, Icolors;
	  	filesize = *(int *)&fileheader[2];
	  	reserve = *(int *)&fileheader[6];
	  	offset = *(int *)&fileheader[10];
	  	headsize = *(int *)&fileheader[14];
	  	width = *(int *)&fileheader[18];
	  	height = *(int *)&fileheader[22];
	  	bitwidth = *(uint16_t *)&fileheader[28];
	  	planes = *(uint16_t *)&fileheader[26];
	  	compression = *(int *)&fileheader[30];
	  	imsize = *(int *)&fileheader[34];
	  	Xresol = *(int *)&fileheader[38];
	  	Yresol = *(int *)&fileheader[42];
	  	Ucolors = *(int *)&fileheader[46];
	  	Icolors = *(int *)&fileheader[50];

	  	cout << "Width of Image" << " " << width << endl;
	  	cout << "Height of Image" << " " << height << endl;
	  	cout << "Bit-width of Image" << " " << bitwidth << endl;
	  	cout << "File-Size in Bytes" << " " << filesize << endl;
	  	cout << "Offset size" << " " << offset << endl;
	  	cout << "Reserve" << " " << reserve << endl;
	  	cout << "Head Size" << " " << headsize << endl;
	  	cout << "Planes" << " " << planes << endl;
	  	cout << "Compression" << " " << compression << endl;
	  	cout << "Imsize" << " " << imsize << endl;
	 	cout << "Xresolv" << " " << Xresol << endl;
	 	cout << "Yresol" << " " << Yresol << endl;
	 	cout << "Ucolors" << " "<< Ucolors << endl;
	 	cout << "Imp Colors" << " " << Icolors << endl;

	 	A.width = width;
	 	A.height = height;
	 	A.fsize = filesize;
	 	A.bitwidth = bitwidth;
	 	A.offset = offset;
	 	A.header_size = headsize;
	 	A.planes = planes;
	 	A.compression = compression;
	 	A.imsize = imsize;
	 	A.Xresol = Xresol;
	 	A.Yresol = Yresol;
	 	A.Ucolors = Ucolors;
	 	A.Icolors = Icolors;

	 	int colors = bitwidth/8;
	 	int offval = 54;

	 	unsigned char *color_table;
	 	color_table = new unsigned char[1024];
	 	int x=1;
	 	int prev = 0;
	 	int count = 0;
	 	while(offval<1078)
	 	{	
	  		offval += sizeof(unsigned char);
	  		if(x%4==0)
	  		{
	  			count += 1;
	  			prev = count;
	  		}
	  		color_table[offval-54] = prev;
	  		x+=1;
	 	}
	 	A.color_table = color_table;
		return A;
	}

	uint8_t *read_img(char* s)
	{	
		uint8_t *imarray;
		FILE* f = fopen(s, "rb");
		FILE* ifile = fopen(s, "rb");
		int colors = A.bitwidth/8;
		int unpadded_row_size = A.width * colors;
		int padded_row_size = (int)(4 * ceil((float)(A.width*colors/4.0f)));
		int totalsize = unpadded_row_size * A.height;
		imarray = new uint8_t[totalsize];
		int i = 0;
		uint8_t* currentpointer = imarray +  (A.height - 1)*unpadded_row_size;
		for(int i=0;i<A.height;i++)
		{
			fseek(ifile, A.offset + (i * padded_row_size), SEEK_SET);
			fread(currentpointer, sizeof(unsigned char), unpadded_row_size, ifile);
			currentpointer -= unpadded_row_size;
			
		}
		// cout << sizeof(imarray) << endl;
		fclose(ifile);
		fclose(f);
		return imarray;

	}

	void writebmp(const char* out, BITMAP A, uint8_t* pixels)
	{
		FILE* outfile = fopen(out, "wb");
		const char *format = "BM";
		int width = A.width;
		int height = A.height;
		uint16_t bitwidth = A.bitwidth;
		int colors = A.bitwidth/8;
		int offset = A.offset;
		fwrite(&format[0], 1, 1, outfile);
		fwrite(&format[1], 1, 1, outfile);
		int padded_row_size = (int )(4 * ceil((float)(width*colors)/4.0f));
		uint32_t filesize = padded_row_size * height + offset;
		fwrite(&filesize, 4, 1, outfile);
		int reserve = 0;
		fwrite(&reserve, 4, 1, outfile);
		int dataoffset = offset;
		fwrite(&dataoffset, 4, 1, outfile);

		int headsize = 40;
		uint16_t planes = 1;
		int compression = A.compression;
		int Xresol = A.Xresol;
		int Yresol = A.Yresol;	
		int Ucolors = A.Ucolors;
		int Icolors = A.Icolors;
		int imsize = width * height * colors;
		
		fwrite(&headsize, 4, 1, outfile);
		fwrite(&width, 4, 1, outfile);
		fwrite(&height, 4, 1, outfile);
		fwrite(&planes, 2, 1, outfile);
		fwrite(&bitwidth, 2, 1, outfile);
		fwrite(&compression, 4, 1, outfile);
		fwrite(&imsize, 4, 1, outfile);
		fwrite(&Xresol, 4, 1, outfile);
		fwrite(&Yresol, 4, 1, outfile);
		fwrite(&Ucolors, 4, 1, outfile);
		fwrite(&Icolors, 4, 1, outfile);

		// cout << "Width of Image" << " " << width << endl;
	 //  	cout << "Height of Image" << " " << height << endl;
	 //  	cout << "Bit-width of Image" << " " << bitwidth << endl;
	 //  	cout << "File-Size in Bytes" << " " << filesize << endl;
	 //  	cout << "Offset size" << " " << offset << endl;
	 //  	cout << "Reserve" << " " << reserve << endl;
	 //  	cout << "Head Size" << " " << headsize << endl;
	 //  	cout << "Planes" << " " << planes << endl;
	 //  	cout << "Compression" << " " << compression << endl;
	 //  	cout << "Imsize" << " " << imsize << endl;
	 // 	cout << "Xresolv" << " " << Xresol << endl;
	 // 	cout << "Yresol" << " " << Yresol << endl;
	 // 	cout << "Ucolors" << " "<< Ucolors << endl;
	 // 	cout << "Imp Colors" << " " << Icolors << endl;

		int i = 0;
		int unpadded_row_size = width*colors;

		unsigned char *color_table_vec = A.color_table;

		if(colors==1)
		{	
			fwrite(color_table_vec, sizeof(unsigned char), offset-54, outfile);
		}

		for(int i=0;i<height;i++)
		{
			int pixeloffset = ((height - i - 1) * unpadded_row_size);
			fwrite(&pixels[pixeloffset], 1, padded_row_size, outfile);
		}
		fclose(outfile);
	}

	uint8_t *gray_scale(uint8_t *img, uint8_t *gs_img, uint16_t bit_width, uint32_t w, uint32_t h, BITMAP& A, string file)
	{	
		if(bit_width>8)
		{	
			// cout<<h<<" "<<flush;
			for(int i=0;i<h;i++)
			{
				for(int j=0;j<w;j++)
				{
					gs_img[i*w+j] = (floor(0.3*img[(3*i)*w+3*j+2]+0.59*img[(3*i)*w+3*j+1]+0.11*img[(3*i)*w+3*j]));
				}	
			}
			A.bitwidth = 8;
			A.offset  = 1078;
			A.Ucolors = 256;
			img = gs_img;	
		}
		string savefile = "./output/" + file + "_grey.bmp";
		char *out = &savefile[0];
		writebmp(out,A,img);
		return img; 
		
	}

	void transpose(uint8_t *imarray, BITMAP A, string file)
	{
		uint8_t *tdata;
		int width = A.width;
		int height = A.height;
		tdata = new uint8_t[width * height];
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				tdata[j*height + i] = imarray[i*width + j]; 
			}
		}
		int temp = A.width;
		A.width = A.height;
		A.height = temp;
		string savefile = "./output/" + file + "_transpose.bmp";
		char *out = &savefile[0];
		writebmp(out,A,tdata);
	}

	void convert1Dto2D(uint8_t* imarray,uint32_t width, uint32_t height, uint8_t **rdata)
	{
		int n = height;
		int m = width;
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<m;j++)
				rdata[i][j] = imarray[i*width + j];
		}
	}

	double linearInterpolation(double x1, double f_x1, double x2, double f_x2, double x)
	{
		double result = (x - x1)/(x2-x1)*f_x2  + (x2-x)/(x2-x1)*f_x1;
		return result;
	}
	uint8_t interpolate(float a, float b,uint8_t ** imarray, uint8_t **data, int n, int m)
	{
		int x = floor(a);
		int y = floor(b);
		int x1 = max(0, x - 1);
		int y1 = max(0, y - 1);
		int x2 = min(n - 1, x + 1);
		int y2 = min(m - 1, y + 1);


		uint8_t q11, q21, q12, q22;
		q11 = imarray[x1][y1];
		q12 = imarray[x1][y2];
		q21 = imarray[x2][y1];
		q22 = imarray[x2][y2];
		if(x==x1 and y==y1)
		{
			return q11;
		}
		else if(x==x1 and y==y2)
		{
			return q12;
		}
		else if(x==x2 and y==y1)
		{
			return q21;
		}
		else if(x==x2 and y==y2)
		{
			return q22;
		}
		double R1 = linearInterpolation(x1,q11,x2,q21,x);
		double R2 = linearInterpolation(x1,q12,x2,q22,x);
		double  P =  linearInterpolation(y1,  R1, y2,  R2, y);
		return P;
	}

	void rotateimage(uint8_t *imarray, uint32_t width, uint32_t height, BITMAP& A, int angle, string file)
	{
		uint8_t **rdata;
		rdata = new uint8_t*[height];
		for(int i=0;i<height;i++)
			rdata[i] = new uint8_t[width];
		convert1Dto2D(imarray,width, height,rdata);
		
		int ox = height/2, oy = width/2;

		uint8_t **newdata;
		newdata = new uint8_t*[height];
		for(int i=0;i<height;i++)
			newdata[i] = new uint8_t[width];
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
				newdata[i][j] = 0;
		}

		int n = height;
		int m = width;
		for(int i = 0;i < n;i++)
		{
			for(int j = 0;j< m;j++)
			{
				int r = i - ox, c = j-oy;
				float xold = r * cos(angle * PI/180.0)  + c * sin(angle * PI/180.0);
				float yold = -r * sin(angle * PI/180.0) +  c * cos(angle * PI/180.0);
				float prex = xold + ox;
				float prey = yold + oy;
				if(prex<0 or prex>=n or prey<0 or prey>=m) continue;
				uint8_t pixel = interpolate(prex,prey,rdata,newdata, n, m);
				newdata[i][j] = pixel;
			}
		}		
		uint8_t *res;
		res = new uint8_t[n*m];
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
				res[i*width + j] = newdata[i][j];
		}
		string savefile = "./output/" + file + "_rotate" + to_string(angle) + ".bmp";
		char *out = &savefile[0];
		writebmp(out,A,res);
	}

};



int main()
{
	string file;
	cout << "Enter File Name" << endl;
	cin >> file; 
	string filename = file + ".bmp";
	char *cstr = &filename[0];
	uint8_t *imarray;
	image obj;
	BITMAP A = obj.readbmp(cstr);
	imarray  = obj.read_img(cstr);
	string outfilename = "./output/" + file + ".bmp";
	char *out = &outfilename[0];
	obj.writebmp(out,A,imarray);
	int s = sizeof(imarray)/sizeof(imarray[0]);
	// cout<<"\n Size : "<<s<<"\n "<<flush;
	uint8_t *gs_img;
	gs_img = new unsigned char[A.width*A.height];
	gs_img = obj.gray_scale(imarray, gs_img, A.bitwidth, A.width, A.height, A, file);
	imarray = gs_img;
	obj.transpose(imarray,A, file);
	// int angle;
	// cout << "Enter Required angle" << endl;
	// cin >> angle;
	obj.rotateimage(imarray,A.width, A.height, A, 45, file);
	obj.rotateimage(imarray,A.width, A.height, A, 90, file);
}
