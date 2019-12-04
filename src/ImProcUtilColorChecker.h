#ifndef _COLOR_CHECKER_H_
#define _COLOR_CHECKER_H_

typedef unsigned int uint;
typedef unsigned char byte;

struct color_rgb
{
    byte r;
    byte g;
    byte b;
    uint ch;

    color_rgb()
        : r(0), g(0), b(0), ch(3)
    {
    }

    void fill_pixel(byte* buf)
    {
        *buf = r;
        *(buf+1) = g;
        *(buf+2) = b;
    }

    void set_color(uint incolor)
    {
        r = (incolor >> 16) & 0xFF;
        g = (incolor >>  8) & 0xFF;
        b = (incolor) & 0xFF;
    }
};

struct cchecker_rgb
{
    cchecker_rgb()
        : w(6), h(4)
    {
		colors[0].set_color(0x6f4f38);
		colors[1].set_color(0xceaa99);
		colors[2].set_color(0x5e8fb8);
		colors[3].set_color(0x607c43);
		colors[4].set_color(0x9b97d2);
		colors[5].set_color(0x8ddad6);
		colors[6].set_color(0xd98404);
		colors[7].set_color(0x3764b9);
		colors[8].set_color(0xd55f72);
		colors[9].set_color(0x6d2181);
		colors[10].set_color(0xbcd448);
		colors[11].set_color(0xf0bb30);
		colors[12].set_color(0x004291);
		colors[13].set_color(0x45a53f);
		colors[14].set_color(0xd11b3f);
		colors[15].set_color(0xecd621);
		colors[16].set_color(0xdc41b7);
		colors[17].set_color(0x00acc1);
		colors[18].set_color(0xe8e8e8);
		colors[19].set_color(0xd0ced9);
		colors[20].set_color(0xabb0b4);
		colors[21].set_color(0x7e8a8a);
		colors[22].set_color(0x4e5c5c);
		colors[23].set_color(0x273333);
    }

    color_rgb* get_color(uint ix, uint iy)
    {
        uint x,y;
        
        if(ix>w-1)
            x = w-1;
        else
            x = ix;

        if(iy>h-1)
            y = h-1;
        else
            y = iy;
        return &(colors[y*w+x]);
    }

    uint        w;
    uint        h;
    color_rgb   colors[24];
};
    
class img_rgb
{
public:
    img_rgb(uint iw, uint ih)
        : w(iw), h(ih), sz(iw*ih*3)
    {
        buf = new byte[sz];
        memset(buf, 0xFF, sz);
        gen_colorchecker();
    }

    ~img_rgb()
    {
        delete[] buf;
    }
    
    byte* get_buf()
    {
        return buf;
    }

    void gen_colorchecker(void)
    {
        cchecker_rgb ccrgb;
        uint bw = w/ccrgb.w;
        uint bh = h/ccrgb.h;

        uint x,y;
        color_rgb* rgb;
        byte* ptr;

        for(uint j=0; j<h; j++)
        {
            y = j/bh;
            for(uint i=0; i<w; i++)
            {
                x = i/bw;
                rgb = ccrgb.get_color(x,y);
                ptr = get_pixel(i, j);
                rgb->fill_pixel(ptr);
            }
        }
    }

protected:
    byte* get_pixel(uint x, uint y)
    {
        return buf + y * w * 3 + x * 3;
    }


private:
    uint        w;
    uint        h;
    uint        sz;
    byte*       buf;
};

#endif //_COLOR_CHECKER_H_