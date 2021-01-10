#include "stdafx.h"
#include "filetool.h"

bool bfsvn(const string& filepath)
{
    string backfile = filepath + ".tmp.bak";

    FILE *fp = fopen(filepath.c_str(), "rb");
    FILE *fptmp = fopen(backfile.c_str(), "wb");

    if (fp == NULL || fptmp == NULL)
    {
        cerr << "ERR: ���ļ�ʧ��! �������ļ����ɶ�/д" << endl;

        if (fp != NULL)
        {
            fclose(fp);
        }

        if (fptmp != NULL)
        {
            fclose(fptmp);
        }
        return false;
    }

    int ch;
    int i = 0;
    int tcount = 4;
    int last = 0;
    int n;

    while((ch = fgetc(fp)) != EOF)
    {
        //��������ֱ������
        if ( (ch & 0x80) != 0 )
        {
            fputc(ch, fptmp);
            ch = fgetc(fp);
            fputc(ch, fptmp);
            i += 2;
            continue;
        }

        //ȥ��\r
        if (ch == '\r')
        {
            continue;
        }

        i++;

        //ת��\t
        if (ch == '\t')
        {
            i--;
            int nleft = i % tcount;

            for (n = nleft; n < tcount; n++)
            {
                fputc(' ', fptmp);
            }

            i += tcount - nleft;
        }
        else
        {

            fputc(ch, fptmp);
        }


        //��յ��м���
        if (ch == '\n') 
        {
            last = i;
            i = 0;
        }
    }

    fclose(fp);

    //�ļ�βҪ��һ����
    if( last != 1 )
    {
        fputc('\n', fptmp);
    }

    fclose(fptmp);

    DeleteFile(filepath.c_str());
    MoveFile(backfile.c_str(), filepath.c_str());
    return true;
}