#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
char* srcname;
if (argv[1]) srcname = argv[1];
else {srcname = argv[0];
srcname[strlen(srcname)-3]='o';
srcname[strlen(srcname)-2]='b';
srcname[strlen(srcname)-1]='j';
}

char tarname[strlen(srcname)+1];
tarname[strlen(srcname)+1] = srcname[strlen(srcname)];
for (int i=0; i<strlen(srcname)-3; i++){
	tarname[i]=srcname[i];
}

tarname[strlen(srcname)-3]='h';
tarname[strlen(srcname)-2]='t';
tarname[strlen(srcname)-1]='m';
tarname[strlen(srcname)]='l';
tarname[strlen(srcname)+1]=srcname[strlen(srcname)];

FILE *srcFile;
FILE *tarFile;

printf("\nsource file: %s", srcname);
if (srcFile = fopen(srcname, "r")){ printf(" [OK]\n", srcname);
	printf("target file: %s", tarname);
	if (tarFile = fopen(tarname, "w")){printf(" [OK]\n", tarname);
	int numv=0;//кол-во точек
	int numf=0;//кол-во граней
	char cond=0;
	
	fprintf(tarFile, "<Shape>\n<Appearance>\n<Material></Material>\n</Appearance>\n");
	fprintf(tarFile, "<IndexedFaceSet ccw='true' colorPerVertex='true' convex='true' solid='false'\ncoordIndex='\n");

	//формируем грани
	while (! feof(srcFile))
    {
		switch(fgetc(srcFile)){
			case '\n': cond=1; break;
			case 'v': if(cond==1) cond='v'; else cond=0; break;
			case 'f': if(cond==1) cond='f'; else cond=0; break;
			case ' ': 
				if(cond=='v') numv++;
				if(cond=='f'){numf++;
					for(int c = fgetc(srcFile); c!='\n'; c = fgetc(srcFile)) {
							if(c=='/') while(c!=' ' && c!='\n') c=fgetc(srcFile); if(c=='\n') break;
							//printf("%c", c);
							fprintf(tarFile, "%c", c);
					}
					//printf(" -1\n");
					fprintf(tarFile, " -1\n");
				}
				cond=1; break;
			default: cond=0;
		}
    }
	
	
	fprintf(tarFile, "'>\n<Coordinate point='\n0 0 0,\n");
	
	
	//запись облака точек
	fseek( srcFile , 0 , SEEK_SET );
	while (! feof(srcFile))
    {
		switch(fgetc(srcFile)){
			case '\n': cond=1; break;
			case 'v': if(cond==1) cond='v'; else cond=0; break;
			case 'f': if(cond==1) cond='f'; else cond=0; break;
			case ' ': 
				if(cond=='v') {
					for(int c = fgetc(srcFile); c!='\n'; c = fgetc(srcFile)) {
							if(c=='/') while(c!=' ' && c!='\n') c=fgetc(srcFile); if(c=='\n') break;
							//printf("%c", c);
							fprintf(tarFile, "%c", c);
					}
					//printf(",\n");
					fprintf(tarFile, ",\n");
				}
				cond=1; break;
			default: cond=0;
		}
    }
	fseek( tarFile , -1 , SEEK_CUR );
	fprintf(tarFile, "'></Coordinate>\n</IndexedFaceSet>\n</Shape>\n");
	printf("%d V and %d F detected\n\n", numv, numf);
	fclose(tarFile);
	fclose(srcFile);
}else printf("\nERROR with target file!");}
else{
	printf("\nERROR!! source file is not opened!");
}
return(0);}