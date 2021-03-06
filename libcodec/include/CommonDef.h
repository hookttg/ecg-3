#ifndef CommonDefH
#define CommonDefH

#include <memory.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

//----------------------------------------------------------------
/*inline int      round(double x)
{
    int r = x;
    if(x - (double)r < 0.5)
        return r;
    else
        return r+1;
}*/
void WriteInt(int v, int n_of_bytes, BYTE*& stream);
int ReadInt(int n_of_bytes, BYTE*& stream);
void WriteUnsigned(int v, int n_of_bytes, BYTE*& stream);
int ReadUnsigned(int n_of_bytes, BYTE*& stream);
void ForwardDPCM(short* x, int len);
void InverseDPCM(short* x, int len);
int get_file_size(const char* filename);
bool compare_files(const char* f1, const char* f2);
double calculate_aml(short** seq, int seq_len);
double calculate_aml(short* buffer, int* p, int seq_len);
double lpc0(short* x, unsigned xLen);

template <class P, class Q>
void copy_vector(register P* dest, register Q* source, register int Len)
{
    if (sizeof(P) != sizeof(Q))
        while (Len--) *dest++ = *source++;
    else
        memcpy(dest, source, sizeof(P) * Len);
}

template <class T>
void reset_vector(register T* dest, register int Len)
{
    memset(dest, 0, Len * sizeof(T));
}

template <class T>
void flipud(register T* p, register int Len)
{
    register T val;
    register T* q = p + Len - 1;
    Len /= 2;
    while (Len--)
    {
        val = *p;
        *p++ = *q;
        *q-- = val;
    }
}

template <class T>
void min_max_element(register T* p, register int len, T& min, T& max)
{
    max = min = *p;
    while (--len)
    {
        if (*++p < min)
            min = *p;
        if (*p > max)
            max = *p;
    }
}

template <class T>
T sum(T* p, register int Len)
{
    T sum = 0;
    while (Len--)
        sum += *p++;
    return sum;
}

template <class T>
void bias2unbias(register T* v, register int vLen)
{
    while (vLen--)
        *v++ = (*v >= 0) ? (*v) * 2 : (*v) * (-2) - 1;
}

template <class T>
void unbias2bias(register T* v, register int vLen)
{
    while (vLen--)
        *v++ = (*v % 2) ? (*v) / (-2) - 1 : (*v) / 2;
}

template <class T>
void diff(T* v, int vLen, double a)
{
    T* v_copy = new T[vLen];
    copy_vector(v_copy, v, vLen);
    for (register int i = 1; i < vLen; i++)
        v[i] -= round(a * v_copy[i - 1]);
    delete[] v_copy;
}

template <class T>
void integrate(T* v, int vLen, double a)
{
    for (register int i = 1; i < vLen; i++)
        v[i] += round(a * v[i - 1]);
}

template <class T>
double mean(T* v, int vLen)
{
    double sum = 0;
    int c = vLen;
    while (c--)
        sum += *v++;
    return ((double)sum / (double)vLen);
}

template <class T>
double std_dev(T* v, int vLen)
{
    double m = mean(v, vLen);
    int c = vLen;
    double difSquare = 0;
    while (c--)
    {
        difSquare += (*v - m) * (*v - m);
        v++;
    }
    return sqrt(difSquare / vLen);
}

template <class T>
bool write_array_to_file(T* data, unsigned data_len, char* filename)
{
    FILE* fptr = fopen(filename, "wb");
    if (fptr)
    {
        BYTE size_t = sizeof(T);
        fwrite(&size_t, 1, 1, fptr);
        fwrite(data, size_t, data_len, fptr);
        fclose(fptr);
        return true;
    }
    else
        return false;
}

template <class T>
bool read_array_from_file(T* data, unsigned data_len, char* filename)
{
    FILE* fptr = fopen(filename, "rb");
    if (fptr)
    {
        BYTE size_t = sizeof(T), size_t_file;
        fwrite(&size_t_file, 1, 1, fptr);
        if (size_t != size_t_file)
            return false; //the file doesn't fit array type
        fread(data, size_t, data_len, fptr);
        fclose(fptr);
        return true;
    }
    else
    {
        return false;
    }
}
#endif
