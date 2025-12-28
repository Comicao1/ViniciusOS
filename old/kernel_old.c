#define ATA_PRIMARY_IO 0x1F0
#define ATA_PRIMARY_CTRL 0x3F6

#define ATA_DATA        0x1F0
#define ATA_SECTOR_COUNT 0x1F2
#define ATA_LBA_LOW      0x1F3
#define ATA_LBA_MID      0x1F4
#define ATA_LBA_HIGH     0x1F5
#define ATA_DRIVE_SELECT 0x1F6
#define ATA_COMMAND      0x1F7
#define ATA_STATUS       0x1F7

#define ATA_READ_SECTORS 0x20

#define READ_U8(ptr) (*(ptr)++)
#define READ_U16(ptr) ((uint16_t)((ptr)[0] | ((ptr)[1] << 8))); (ptr) += 2
#define READ_U32(ptr) \
    ({ uint32 __val = (uint32)((ptr)[0] | ((ptr)[1] << 8) | ((ptr)[2] << 16) | ((ptr)[3] << 24)); \
       ptr += 4; __val; })
#define READ_F32(ptr) \
    ({ union { float f; uint8_t b[4]; } __u; \
       __u.b[0] = (ptr)[0]; __u.b[1] = (ptr)[1]; \
       __u.b[2] = (ptr)[2]; __u.b[3] = (ptr)[3]; \
       ptr += 4; __u.f; })

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32;
typedef unsigned long long uint32;

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

float const PI = 3.1415f;

typedef struct {
    float x, y, z;
    uint32 color;
} Vertex3D;

typedef struct {
    int x, y;
    uint32 color;
} Vertex;

typedef struct {
    Vertex3D v0, v1, v2;
    float avg_z;
} Triangle;

typedef struct {
    short v1, v2, v3; // Indices to vertices

} Face;

unsigned char* vga = (unsigned char*)0xA0000;
unsigned char* back_buffer = (unsigned char*)0x90000;
unsigned char* model_data = (unsigned char*)0x100000;


char font8x8_basic[128][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (;)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};

void clear_screen(unsigned char* tela, int width, int height, unsigned char color);
void fill_triangle(unsigned char* tela, int width, int height, Vertex v0, Vertex v1, Vertex v2);
void fill_flat_top_triangle(unsigned char* tela, int width, int height, Vertex v0, Vertex v1, Vertex v2);
void fill_flat_bottom_triangle(unsigned char* tela, int width, int height, Vertex v0, Vertex v1, Vertex v2);
void delay(int count);
void draw_triangle(unsigned char* tela, int width, int height, Vertex v0, Vertex v1, Vertex v2);
void draw_line(unsigned char* tela, int width, int height, int x0, int y0, int x1, int y1, uint32 color);
void put_pixel(unsigned char* tela, short x, short y, int color);
void rotatePoint(int x, int y, int cx, int cy, float angle, Vertex *v0);
float cosseno(float x);
float seno(float x);
int fatorial(int x);
float elevado_a(float base, int expo);
void wait_for_vsync();
void draw_char(unsigned char* tela, int x, int y, char c, int color);
void draw_text(unsigned char* tela, int x, int y, const char* text, int color);
void project_vertex(Vertex3D* v3d, Vertex* v2d, int screen_width, int screen_height, float focal_length);
void rotate_x(Vertex3D* v, float angle_deg);
void rotate_y(Vertex3D* v, float angle_deg);
void rotate_z(Vertex3D* v, float angle_deg);
void translate(Vertex3D* v, float tx, float ty, float tz);
void wait_retrace();
void clear_buffer(unsigned char color);
void blit_buffer();
void ata_wait();
void ata_read_sectors(unsigned int lba, unsigned char* buffer, unsigned char sector_count);

void kernel_main() {
    ata_read_sectors(67, model_data, 20);

    //read model

    Vertex3D vertices[150];
    Face faces[260];

    unsigned char* data = model_data;
    data += 7;
    char n_obj = READ_U8(data);
    short n_vertices = READ_U16(data);
    short n_faces = READ_U16(data);

    for(int i = 0; i < n_vertices; i++){
        float x = READ_F32(data);
        float y = READ_F32(data);
        float z = READ_F32(data);
        vertices[i].x = x;
        vertices[i].y = y;
        vertices[i].z = z;
    }
    for(int i = 0; i < n_faces; i++){
        char numberFaces = READ_U8(data);
        int v[4] = {0}, vt[4] = {0}, vn[4] = {0};
        //printf("%d \n", numberFaces);
        for(int j = 0; j < numberFaces; j++){
            v[j] = READ_U16(data);
            vt[j] = READ_U16(data);
            vn[j] = READ_U16(data);
            v[j] -= 1;
            vt[j] -= 1;
            vn[j] -= 1;
        }
        faces[i].v1 = v[0];
        faces[i].v2 = v[1];
        faces[i].v3 = v[2];
    }
    
    
    //read model end

    Vertex3D cube_faces[36] = {
        // FRONT (Z+), RED
        { -0.5f, -0.5f,  0.5f, 4 },
        {  0.5f, -0.5f,  0.5f, 4 },
        {  0.5f,  0.5f,  0.5f, 4 },

        { -0.5f, -0.5f,  0.5f, 4 },
        {  0.5f,  0.5f,  0.5f, 4 },
        { -0.5f,  0.5f,  0.5f, 4 },

        // BACK (Z-), GREEN
        {  0.5f, -0.5f, -0.5f, 5 },
        { -0.5f, -0.5f, -0.5f, 5 },
        { -0.5f,  0.5f, -0.5f, 5 },

        {  0.5f, -0.5f, -0.5f, 5 },
        { -0.5f,  0.5f, -0.5f, 5 },
        {  0.5f,  0.5f, -0.5f, 5 },

        // LEFT (X-), BLUE
        { -0.5f, -0.5f, -0.5f, 6 },
        { -0.5f, -0.5f,  0.5f, 6 },
        { -0.5f,  0.5f,  0.5f, 6 },

        { -0.5f, -0.5f, -0.5f, 6 },
        { -0.5f,  0.5f,  0.5f, 6 },
        { -0.5f,  0.5f, -0.5f, 6 },

        // RIGHT (X+), YELLOW
        { 0.5f, -0.5f,  0.5f, 7 },
        { 0.5f, -0.5f, -0.5f, 7 },
        { 0.5f,  0.5f, -0.5f, 7 },

        { 0.5f, -0.5f,  0.5f, 7 },
        { 0.5f,  0.5f, -0.5f, 7 },
        { 0.5f,  0.5f,  0.5f, 7 },

        // TOP (Y+), CYAN
        { -0.5f, 0.5f,  0.5f, 8 },
        {  0.5f, 0.5f,  0.5f, 8 },
        {  0.5f, 0.5f, -0.5f, 8 },

        { -0.5f, 0.5f,  0.5f, 8 },
        {  0.5f, 0.5f, -0.5f, 8 },
        { -0.5f, 0.5f, -0.5f, 8 },

        // BOTTOM (Y-), MAGENTA
        { -0.5f, -0.5f, -0.5f, 9 },
        {  0.5f, -0.5f, -0.5f, 9 },
        {  0.5f, -0.5f,  0.5f, 9 },

        { -0.5f, -0.5f, -0.5f, 9 },
        {  0.5f, -0.5f,  0.5f, 9 },
        { -0.5f, -0.5f,  0.5f, 9 }
    };

    Triangle tris_transformed[260]; // 12 triangles in cube

    Vertex original_v0 = { 100, 50, 4 };
    Vertex original_v1 = { 150, 100, 3 };
    Vertex original_v2 = { 50, 100, 2 };

    float angle = 10.0f;


    while (1) {
        //clear_screen(vga, 320, 200, 0);
        clear_buffer(0);
        draw_text(back_buffer, 10, 3, "ViniciusOS - Kernel Mode (32 BITS)", 15);
        draw_text(back_buffer, 10, 15, "VGA 320x200 256 Cores", 15);
        angle += 0.4f;
        if(angle >= 360.0f) angle = 0.0f;


        for(int i = 0; i < n_faces; i++) {

            tris_transformed[i].v0 = vertices[faces[i].v1];
            tris_transformed[i].v1 = vertices[faces[i].v2];
            tris_transformed[i].v2 = vertices[faces[i].v3];
            tris_transformed[i].v0.color = 14;
            tris_transformed[i].v1.color = 14;
            tris_transformed[i].v2.color = 14;

            // tris_transformed[i].v0 = cube_faces[i*3];
            // tris_transformed[i].v1 = cube_faces[i*3 + 1];
            // tris_transformed[i].v2 = cube_faces[i*3 + 2];


            rotate_y(&tris_transformed[i].v0, angle);
            rotate_y(&tris_transformed[i].v1, angle);
            rotate_y(&tris_transformed[i].v2, angle);

            // rotate_x(&tris_transformed[i].v0, angle);
            // rotate_x(&tris_transformed[i].v1, angle);
            // rotate_x(&tris_transformed[i].v2, angle);

            rotate_z(&tris_transformed[i].v0, angle);
            rotate_z(&tris_transformed[i].v1, angle);
            rotate_z(&tris_transformed[i].v2, angle);

            translate(&tris_transformed[i].v0, 0, 0, 2.4f);
            translate(&tris_transformed[i].v1, 0, 0, 2.4f);
            translate(&tris_transformed[i].v2, 0, 0, 2.4f);

            tris_transformed[i].avg_z = (tris_transformed[i].v0.z + tris_transformed[i].v1.z + tris_transformed[i].v2.z) / 3.0f;
        }


        for(int i = 0; i < n_faces-1; i++) {
            for(int j = i+1; j < n_faces; j++) {
                if(tris_transformed[i].avg_z < tris_transformed[j].avg_z) {
                    Triangle tmp = tris_transformed[i];
                    tris_transformed[i] = tris_transformed[j];
                    tris_transformed[j] = tmp;
                }
            }
        }

        for(int i = 0; i < n_faces; i++) {
            Vertex pv0, pv1, pv2;
            project_vertex(&tris_transformed[i].v0, &pv0, 320, 200, 100.0f);
            project_vertex(&tris_transformed[i].v1, &pv1, 320, 200, 100.0f);
            project_vertex(&tris_transformed[i].v2, &pv2, 320, 200, 100.0f);

            //draw_triangle(back_buffer, 320, 200, pv0, pv1, pv2);
            fill_triangle(back_buffer, 320, 200, pv0, pv1, pv2);
        }
        blit_buffer();
        wait_retrace();
        //delay(4);
    }
}

void clear_buffer(unsigned char color) {
    uint32 color32 = color | (color << 8) | (color << 16) | (color << 24);
    uint32* buffer32 = (uint32*)back_buffer;
    int total_dwords = (320 * 200) / 4;

    for (int i = 0; i < total_dwords; i++) {
        buffer32[i] = color32;
    }

    for (int i = total_dwords * 4; i < 320 * 200; i++) {
        back_buffer[i] = color;
    }
}

void blit_buffer() {
    uint32* vga32 = (uint32*)vga;
    uint32* buffer32 = (uint32*)back_buffer;
    int total_dwords = (320 * 200) / 4;

    for (int i = 0; i < total_dwords; i++) {
        vga32[i] = buffer32[i];
    }

    for (int i = total_dwords * 4; i < 320 * 200; i++) {
        vga[i] = back_buffer[i];
    }
}

void clear_screen(unsigned char* tela, int width, int height, unsigned char color) {
    uint32 color32 = color | (color << 8) | (color << 16) | (color << 24);
    uint32* tela32 = (uint32*)tela;
    int total_dwords = (width * height) / 4;

    for (int i = 0; i < total_dwords; i++) {
        tela32[i] = color32;
    }

    // Handle remaining bytes if total pixels not divisible by 4
    for (int i = total_dwords * 4; i < width * height; i++) {
        tela[i] = color;
    }
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Wait until BSY clears and DRQ sets
void ata_wait() {
    while (inb(ATA_STATUS) & 0x80);   // Wait for BSY to clear
    while (!(inb(ATA_STATUS) & 0x08)); // Wait for DRQ to set
}

// Read one 512-byte sector into buffer
void ata_read_sectors(unsigned int lba, unsigned char* buffer, unsigned char sector_count) {
    outb(ATA_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));  // LBA mode
    outb(ATA_SECTOR_COUNT, sector_count);                // 20 sectors
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_COMMAND, ATA_READ_SECTORS);

    for (int s = 0; s < sector_count; ++s) {
        ata_wait();  // Wait for each sector

        // Read one sector (256 words = 512 bytes)
        for (int i = 0; i < 256; ++i) {
            unsigned short data;
            asm volatile ("inw %1, %0" : "=a"(data) : "Nd"(ATA_DATA));
            buffer[s * 512 + i * 2] = data & 0xFF;
            buffer[s * 512 + i * 2 + 1] = data >> 8;
        }
    }
}

void wait_retrace() {
    while (inb(0x3DA) & 0x08);   // Wait for end of retrace
    while (!(inb(0x3DA) & 0x08)); // Wait for start of retrace
}

void project_vertex(Vertex3D* v3d, Vertex* v2d, int screen_width, int screen_height, float focal_length) {
    if (v3d->z <= 0.1f) v3d->z = 0.1f;

    v2d->x = (int)(screen_width / 2 + (v3d->x * focal_length) / v3d->z);
    v2d->y = (int)(screen_height / 2 - (v3d->y * focal_length) / v3d->z);
    v2d->color = v3d->color;
}

void rotate_x(Vertex3D* v, float angle_deg) {
    float rad = angle_deg * PI / 180.0f;
    float cos_a = cosseno(rad);
    float sin_a = seno(rad);

    float y = v->y;
    float z = v->z;

    v->y = y * cos_a - z * sin_a;
    v->z = y * sin_a + z * cos_a;
}

void rotate_y(Vertex3D* v, float angle_deg) {
    float rad = angle_deg * PI / 180.0f;
    float cos_a = cosseno(rad);
    float sin_a = seno(rad);

    float x = v->x;
    float z = v->z;

    v->x = x * cos_a - z * sin_a;
    v->z = x * sin_a + z * cos_a;
}

void rotate_z(Vertex3D* v, float angle_deg) {
    float rad = angle_deg * PI / 180.0f;
    float cos_a = cosseno(rad);
    float sin_a = seno(rad);

    float x = v->x;
    float y = v->y;

    v->x = x * cos_a - y * sin_a;
    v->y = x * sin_a + y * cos_a;
}

void translate(Vertex3D* v, float tx, float ty, float tz) {
    v->x += tx;
    v->y += ty;
    v->z += tz;
}

void draw_text(unsigned char* tela, int x, int y, const char* text, int color) {
    while (*text) {
        draw_char(tela, x, y, *text, color);
        x += 8;
        text++;
    }
}

void draw_char(unsigned char* tela, int x, int y, char c, int color) {
    for (int row = 0; row < 8; row++) {
        unsigned char byte = font8x8_basic[(unsigned char)c][row];
        for (int col = 0; col < 8; col++) {
            if (byte & (1 << (7 - col))) {
                put_pixel(tela, x - col, y + row, color);
            }
        }
    }
}

float normalizeRadians(float x) {
    const float TWO_PI = 2.0f * PI;
    while (x > PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;
    return x;
}

float elevado_a(float base, int expo){
    float resultado = 1.0;
    for(int i = 0; i < expo; i++){
        resultado *= base;
    }
    return resultado;
}

int fatorial(int x){
    int resultado = 1;
    for(int i = 2; i <= x; i++){
        resultado *= i;
    }
    return resultado;
}

float seno(float x){
    x = normalizeRadians(x);
    float resultado = 0.0;
    for(int i = 0; i < 6; i++){
        int exp = 2 * i + 1;
        float termo = elevado_a(x, exp) / fatorial(exp);
        if(i % 2 == 0)
            resultado += termo;
        else
            resultado -= termo;
    }
    return resultado;
}

float cosseno(float x){
    x = normalizeRadians(x);
    float resultado = 0.0;
    for(int i = 0; i < 6; i++){
        int exp = 2 * i;
        float termo = elevado_a(x, exp) / fatorial(exp);
        if(i % 2 == 0)
            resultado += termo;
        else
            resultado -= termo;
    }
    return resultado;
}

void rotatePoint(int x, int y, int cx, int cy, float angle, Vertex *v0) {
    float radians = angle * PI / 180.0f;

    float cos_theta = cosseno(radians);
    float sin_theta = seno(radians);

    float dx = (float)(x - cx);
    float dy = (float)(y - cy);

    float rx = dx * cos_theta - dy * sin_theta;
    float ry = dx * sin_theta + dy * cos_theta;

    v0->x = (int)(rx + cx);
    v0->y = (int)(ry + cy);
}


void put_pixel(unsigned char* tela, short x, short y, int color){
    if (x < 0 || x >= 320 || y < 0 || y >= 200) return;
    tela[y * 320 + x] = color;
}

void draw_line(unsigned char* tela, int width, int height, int x0, int y0, int x1, int y1, uint32 color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int abs_dx = dx < 0 ? -dx : dx;
    int abs_dy = dy < 0 ? -dy : dy;

    int sx = dx > 0 ? 1 : -1;
    int sy = dy > 0 ? 1 : -1;

    int err = (abs_dx > abs_dy ? abs_dx : -abs_dy) / 2;
    int e2;

    while (1) {
        if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height)
            //tela[y0 * width + x0] = color;
            put_pixel(tela, x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        e2 = err;
        if (e2 > -abs_dx) {
            err -= abs_dy;
            x0 += sx;
        }
        if (e2 < abs_dy) {
            err += abs_dx;
            y0 += sy;
        }
    }
}

void draw_triangle(unsigned char* tela, int width, int height, Vertex v0, Vertex v1, Vertex v2) {
    draw_line(tela, width, height, v0.x, v0.y, v1.x, v1.y, v0.color);
    draw_line(tela, width, height, v1.x, v1.y, v2.x, v2.y, v1.color);
    draw_line(tela, width, height, v2.x, v2.y, v0.x, v0.y, v2.color);
}

void delay(int count) {
    for (volatile int i = 0; i < count * 100000; i++) {}
}

void fill_flat_bottom_triangle(unsigned char* tela, int width, int height, Vertex v0, Vertex v1, Vertex v2) {
    float invslope1 = (float)(v1.x - v0.x) / (v1.y - v0.y);
    float invslope2 = (float)(v2.x - v0.x) / (v2.y - v0.y);

    float curx1 = v0.x;
    float curx2 = v0.x;

    for (int scanlineY = v0.y; scanlineY <= v1.y; scanlineY++) {
        int startX = (int)curx1;
        int endX = (int)curx2;

        if (startX > endX) {
            int tmp = startX;
            startX = endX;
            endX = tmp;
        }

        for (int x = startX; x <= endX; x++) {
            if (x >= 0 && x < width && scanlineY >= 0 && scanlineY < height)
                //tela[scanlineY * width + x] = v0.color;
                put_pixel(tela, x, scanlineY, v0.color);
        }
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void fill_flat_top_triangle(unsigned char* tela, int width, int height, Vertex v0, Vertex v1, Vertex v2) {
    float invslope1 = (float)(v2.x - v0.x) / (v2.y - v0.y);
    float invslope2 = (float)(v2.x - v1.x) / (v2.y - v1.y);

    float curx1 = v2.x;
    float curx2 = v2.x;

    for (int scanlineY = v2.y; scanlineY >= v0.y; scanlineY--) {
        int startX = (int)curx1;
        int endX = (int)curx2;

        if (startX > endX) {
            int tmp = startX;
            startX = endX;
            endX = tmp;
        }

        for (int x = startX; x <= endX; x++) {
            if (x >= 0 && x < width && scanlineY >= 0 && scanlineY < height)
                //tela[scanlineY * width + x] = v2.color;
                put_pixel(tela, x, scanlineY, v2.color);
        }
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void fill_triangle(unsigned char* tela, int width, int height, Vertex v0, Vertex v1, Vertex v2) {
    // Sort by Y
    if (v0.y > v1.y) { Vertex temp = v0; v0 = v1; v1 = temp; }
    if (v0.y > v2.y) { Vertex temp = v0; v0 = v2; v2 = temp; }
    if (v1.y > v2.y) { Vertex temp = v1; v1 = v2; v2 = temp; }

    if (v1.y == v2.y) {
        fill_flat_bottom_triangle(tela, width, height, v0, v1, v2);
    } else if (v0.y == v1.y) {
        fill_flat_top_triangle(tela, width, height, v0, v1, v2);
    } else {
        // Split the triangle
        Vertex v3;
        v3.y = v1.y;
        float alpha = (float)(v1.y - v0.y) / (float)(v2.y - v0.y);
        v3.x = v0.x + (int)(alpha * (v2.x - v0.x));
        v3.color = v0.color;

        fill_flat_bottom_triangle(tela, width, height, v0, v1, v3);
        fill_flat_top_triangle(tela, width, height, v1, v3, v2);
    }
}

