#define _CRT_SECURE_NO_WARNINGS//ѹ�ƺ�
#include<graphics.h>
#include<stdio.h>
#include<windows.h>
struct rgb
{	
	unsigned int r;
	unsigned int g;
	unsigned int b;
};
void image()
{	
	struct rgb *RGB;
	IMAGE picture;
	DWORD *pic;
	FILE *fp_rgb;
	char pic_name[30];
	char pic_name1[30];
	printf("������ͼƬ��Ϣ�磨yyk.jpg��\n");
	scanf("%s", pic_name);
	printf("�����뱣����ļ���Ϣ\n");
	scanf("%s", pic_name1);	
	loadimage(&picture, pic_name);//��ȡͼƬ��Ϣ
	pic = GetImageBuffer(&picture);//��ȡ�豸�Դ�
	RGB = (struct rgb *)malloc(sizeof(struct rgb));//�����ڴ�
	if (!(fp_rgb = fopen(pic_name1, "w")))//��ͼƬ�ļ�	
	{	
		printf("error");	
		system("pause");	
		return;
	}
	fprintf(fp_rgb, "WIDTH=12;\n");
	fprintf(fp_rgb, "DEPTH=327680;\n");
	fprintf(fp_rgb, "ADDRESS_RADIX=HEX;\n");
	fprintf(fp_rgb, "DATA_RADIX=HEX;\n");
	fprintf(fp_rgb, "CONTENT\n");
	fprintf(fp_rgb, "BEGIN\n");
	for (int i = 0; i< 327680; ++i)	
	{
		RGB->r = ((unsigned)GetRValue(pic[i]) >> 4); //ȡ��4λ
		RGB->g = ((unsigned)GetGValue(pic[i]) >> 4);//ȡ��4λ
		RGB->b = ((unsigned)GetBValue(pic[i]) >> 4);//ȡ��4λ
		fprintf(fp_rgb, "%06x: ",i);
		fprintf(fp_rgb, "%x",RGB->r);
		fprintf(fp_rgb, "%x", RGB->g);
		fprintf(fp_rgb, "%x;\n", RGB->b);
	}
	fprintf(fp_rgb, "END;\n");
	fclose(fp_rgb);//�ر�	
	free(RGB);	
	printf("SUCESS!");
} 
int main()
{	
	image();
	system("pause");
	return 0;
}
