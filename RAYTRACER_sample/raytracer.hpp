#pragma once
/*------------------------------------------------------------------------
  This is based on Paul Heckbert's "business card raytracer".
  His site is: http://www.cs.cmu.edu/~ph/

  I modified it a lot adding colors/materials, arbitrary sphere
  positions, etc.
  
  I also added some comments and made it readable. The original
  code was designed to be small enough to print on the back of
  a business card (hence the name) so it was very hard to read.
  
  FTB.
------------------------------------------------------------------------*/
#include <cmath>
#include <cstdint>

extern "C" {
	void draw_pixel(int x, int y, int r, int g, int b);
	void draw_text(int x, int y, const char* t);
	uint32_t millis(void);
};

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
// #define FAST_INV_SQRT
// Because precision is not enough, I do not use it

#if defined(SIG_RX140) || defined(SIG_RX231) || defined(SIG_RX64M) || defined(SIG_RX71M) || defined(SIG_RX65N) || defined(SIG_RX24T) || defined(SIG_RX26T) || defined(SIG_RX66T) || defined(SIG_RX72M) || defined(SIG_RX72T) || defined(SIG_RX72N)
static inline float sqrtf_(float x)
{
    __asm __volatile(
        "fsqrt %0, %0\n" \
        : "+r"(x) \
    );
    return x;
}
#else
static inline float sqrtf_(float x) { return sqrtf(x); }
#endif

#if defined(SIG_RX140) || defined(SIG_RX231) || defined(SIG_RX621) || defined(SIG_RX62N) || defined(SIG_RX64M) || defined(SIG_RX71M) || defined(SIG_RX65N) || defined(SIG_RX24T) || defined(SIG_RX26T) || defined(SIG_RX66T) || defined(SIG_RX72M) || defined(SIG_RX72T) || defined(SIG_RX72N)
static inline int ceilf_(float x)
{
    int y;
    __asm __volatile(
        "pushc fpsw\n"
        "mvtc #0b10, fpsw\n"
        "round %0, %0\n"
        "popc fpsw\n"
        : "=r"(y) \
        : "r"(x) \
    );
    return y;
}
#else
static inline int ceilf_(float x) { return ceilf(x); }
#endif

/*------------------------------------------------------------------------
  Values you can play with...
------------------------------------------------------------------------*/

// Position of the camera (nb. 'Z' is up/down)
static constexpr float cameraX = 0.0f;
static constexpr float cameraY = 0.0f;
static constexpr float cameraZ = 3.0f;

// What the camera is pointing at
static constexpr float targetX = 1.0f;
static constexpr float targetY = 8.0f;
static constexpr float targetZ = 4.0f;

// We cast this many rays per pixel for stochastic antialiasing and soft-shadows. 
//
// Large numbers produce a nicer image but it runs a lot slower
//static constexpr int raysPerPixel = 4;

// The camera's field of view, smaller=>zoom, larger=>wide angle
static constexpr float fov = 0.45f;

// The size of the soft shadow, larger=>wider area
static constexpr float shadowRegion = 0.125f;

/*------------------------------------------------------------------------
  Materials
------------------------------------------------------------------------*/
static constexpr float ambient = 0.05f;
static constexpr float materials[] = {
// R,    G,    B,   REFLECTIVITY
  0.8f, 0.8f, 0.8f,   0.5f,    // Mirror
//  1.0f, 0.0f, 0.0f,   0.3f,    // Red
  0.0f, 1.0f, 0.0f,   0.2f,    // Green
//  0.0f, 0.0f, 0.1f,   0.3f     // Dark blue
  0.0f, 0.8f, 0.8f,   0.3f     // Cyan
};

/*------------------------------------------------------------------------
  The spheres in the world
------------------------------------------------------------------------*/
#define NUM_SPHERES 4
static constexpr float spheres[] = {
// center  radius material
   5,15,8,   5,     0,
  -6,12,4,   3,     0,
   1,10,2,   2,     1,
   0, 9,5,   1,     2
};

#ifdef FAST_INV_SQRT
static inline float Q_rsqrt( float number )
{
	union {
		float f;
		uint32_t i;
	} conv;
	
	float x2;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	conv.f  = number;
	conv.i  = 0x5f3759df - ( conv.i >> 1 );
	conv.f  = conv.f * ( threehalfs - ( x2 * conv.f * conv.f ) );
	return conv.f;
}
#endif

/*------------------------------------------------------------------------
  A 3D vector class
------------------------------------------------------------------------*/
struct vec3 {
  float x,y,z;  // Vector has three float attributes.
  vec3(){}
  vec3(float a, float b, float c){x=a;y=b;z=c;}
  vec3 operator+(const vec3& v) const { return vec3(x+v.x,y+v.y,z+v.z);  }    // Vector add
  vec3 operator-(const vec3& v) const { return (*this)+(v*-1);           }    // Vector subtract
  vec3 operator*(float s)       const { return vec3(x*s,y*s,z*s);        }    // Vector scale
  float operator%(const vec3& v)const { return x*v.x+y*v.y+z*v.z;        }    // Scalar product
  vec3 operator^(const vec3& v) const { return vec3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);  } // Vector product

#ifdef FAST_INV_SQRT
  vec3 operator!()              const { return *this*(Q_rsqrt(*this%*this)); }  // Normalized
#else
  vec3 operator!()              const { return *this*(1.0f/sqrtf_(*this%*this)); }  // Normalized vector
#endif
  void operator+=(const vec3& v)      { x+=v.x;  y+=v.y;  z+=v.z;        }
  void operator*=(float s)            { x*=s;    y*=s;    z*=s;          }
};

// A ray...
struct ray {
  // This occupies 24 bytes - you could only fit 20 of these into
  // a Tiny85 even if you could use the entire RAM (which you can't...)
  vec3 o;  // Origin
  vec3 d;  // Direction
};

/*------------------------------------------------------------------------
  Intersect a ray with the world
  Return 'SKY' if no hit was found but ray goes upward
  Return 'FLOOR' if no hit was found but ray goes downward towards the floor
  Return a material index if a hit was found

  Distance to the hit is returned in 'distance'. 
  The surface normal at the hit is returned in 'normal'
------------------------------------------------------------------------*/
// Values for 'SKY' and 'FLOOR'
static constexpr uint8_t SKY=255;
static constexpr uint8_t FLOOR=254;

uint8_t trace(const ray& r, float& distance, vec3& normal)
{
  // Assume we didn't hit anything
  uint8_t result = SKY;

  // Does the ray go downwards?
  float d = -r.o.z/r.d.z;
  if (d > 0.01f) {
    // Yes, assume it hits the floor
    result = true;
    distance = d;
    result = FLOOR;
    normal = vec3(0.0f,0.0f,1.0f);
  }

  // Test the objects in the scene to see if there's anything in the way
  for (uint8_t i=0; i<NUM_SPHERES; ++i) {
    vec3 oc = r.o;
    // Read a sphere from progmem, calculate vector 'oc'
    const float* n = spheres+(i*5);
    oc.x -= *n++;
    oc.y -= *n++;
    oc.z -= *n++;
    d = *n++;  // Radius

    // Ray-sphere intersection test
    // Math is here: http://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    const float b = r.d%oc;        // I.(o-c)
    const float c = (oc%oc)-(d*d); // (o-c).(o-c) - r^2

    // Does the ray hit the sphere?
    d = (b*b)-c;
    if (d > 0) {
      // Yes, compute the distance to the hit
      d = (-b)-sqrtf_(d);

      // Is it the closest hit so far?
      if ((d > 0.01) and ((result==SKY) or (d<distance))) {
        // Yes, save results
        distance = d;
        normal = !(oc+r.d*d);
        result = static_cast<uint8_t>(*n);  // The sphere's material
      }
    }
  }
  return result;
}

float raise(float p, uint8_t n)
{
  while (n--) {
    p = p*p;
  }
  return p;
}

/*----------------------------------------------------------
  Small, fast pseudo-random number generator
  
  I found this in a forum and I'm not sure who originally
  wrote it. It works very well though....
 
  If you wrote this then get in touch and I'll put
  your name here. :-)                              FTB.
----------------------------------------------------------*/
uint8_t randomByte()
{
  static uint8_t rngA, rngB, rngC, rngX;
  ++rngX;                        // X is incremented every round and is not affected by any other variable
  rngA = (rngA ^ rngC ^ rngX);       // note the mix of addition and XOR
  rngB = (rngB + rngA);            // And the use of very few instructions
  rngC = ((rngC + (rngB >> 1)) ^ rngA);  // the right shift is to ensure that high-order bits from B can affect  
  return rngC;
}

// A random float in the range [-0.5 ... 0.5]  (more or less)
float randomFloat()
{
  char r = char(randomByte());
  return float(r)/256.0f;
}
#define RF randomFloat()
#define SH (RF*shadowRegion)

/*------------------------------------------------------------------------
  Sample the world and return the pixel color for a ray
------------------------------------------------------------------------*/
float sample(ray& r, vec3& color)
{
  // See if the ray hits anything in the world
  float t;  vec3& n = color;      // RAM is tight, use 'color' as temp workspace
  const uint8_t hit = trace(r,t,n);

  // Did we hit anything
  if (hit == SKY) {
    // Generate a sky color if the ray goes upwards without hitting anything
    color = vec3(0.1f,0.0f,0.3f) + vec3(.7f,.2f,0.5f)*raise(1.0f-r.d.z,2);
    return 0.0f;
  }

  // New ray origin
  r.o += r.d*t;

  // Half vector
  const vec3 half = !(r.d + n * ((n % r.d) * -2.0f));

// Vector that points towards the light
  r.d = vec3(9.0f + SH, 6.0f + SH, 16.0f); // Where the light is
  r.d = !(r.d-r.o);          // Normalized light vector

  // Lambertian factor
  float d = r.d%n;    // Light vector % surface normal

  // See if we're in shadow
  if ((d<0) or (trace(r,t,n)!=SKY)) {
    d = 0;
  }

  // Did we hit the floor?
  if (hit == FLOOR) {
    // Yes, generate a floor color
    d=(d*0.2f)+0.1f;   t=d*3.0f;  // d=dark, t=light
    color = vec3(t,t,t);       // Assume grey color
    t = 1.0f/5.0f;     // Floor tiles are 5m across
//    int fx = int(ceil(r.o.x*t));
//    int fy = int(ceil(r.o.y*t));
//    bool dark = ((fx+fy)&1)!=0;  // Light or dark color?
    bool dark = (((int)(ceilf_(r.o.x*t)+ceilf_(r.o.y*t)))&1);  // Light or dark color? -> fix for AVR compiler
    if (dark) { color.y = color.z = d; }        // g+b => dark => 'red'
    return 0;
  }

  // No, we hit the scene, read material color from progmem
  const float* mat = materials + (hit * 4);
  color.x = *mat++;
  color.y = *mat++;
  color.z = *mat++;
 
  // Specular light in 't'
  t = d;
  if (t > 0) {
    t = raise(r.d%half,5);
  }

  // Calculate total color using diffuse and specular components
  color *= d*d+ambient;  // Ambient+diffuse
  color += vec3(t,t,t);  // Specular

  // We need to trace a reflection ray...need to modify 'r' for the recursion
  r.d = half;
  return *mat;    // Reflectivity of this material
}

/*------------------------------------------------------------------------
  Raytrace the entire image
------------------------------------------------------------------------*/
void doRaytrace(int raysPerPixel = 4, int dw = 320, int dh = 240, int q = 1)
{
  // Trace it
  int dw2=dw/2;;
  int dh2=dh/2;
  const float pixel =  fov/float(dh2);    // Size of one pixel on screen

  // Position/target of camera
  const vec3 camera = vec3(cameraX,cameraY,cameraZ);
  const vec3 target = vec3(targetX,targetY,targetZ);
  
  auto t = millis();

  for (int y=0; y<dh; y+=q) {
    for (int x=0; x<dw; x+=q) {
      vec3 acc(0,0,0);     // Color accumulator
      for (int p=raysPerPixel; p--;) {
        ray r;  vec3 temp;
        auto xpos = static_cast<float>(x - dw2);
		auto ypos = static_cast<float>(dh2 - y);
        if (raysPerPixel>1) { xpos+=RF; ypos+=RF; }       // Stochastic antialiasing when RPP > 1

        // Calculate a ray through this pixel
        temp = !(target-camera);
        vec3& right = r.o;   right = !(temp^vec3(0.0f, 0.0f, 1.0f));
        vec3& up = r.d;      up = !(right^temp);
        r.d = !(temp + ((right*xpos)+(up*ypos))*pixel);  // Ray direction
        r.o = camera;                                    // Ray starts at the camera
        
        // Sample the world, accumulate the color returned
        vec3& color = temp;
        float reflect1 = sample(r,color);
        acc += color;
        // 'sample()' would normally be recursive but there's not enough RAM to do that on a Tiny85...
        if (reflect1 > 0) {
          // ...so we do the 'recursion' manually
          float reflect2 = sample(r,color);
          acc += color*reflect1;
          if (reflect2 > 0) {
            // ...3 levels deep
            sample(r,color);
            acc += color*(reflect1*reflect2);
          }
        }
      }
      
      // Output the pixel
      acc = acc * (255.0f / static_cast<float>(raysPerPixel));
      int r = acc.x;    if (r>255) { r=255; }
      int g = acc.y;    if (g>255) { g=255; }
      int b = acc.z;    if (b>255) { b=255; }
	  draw_pixel(x, y, r, g, b);
    }

//	char buf[50];
//	auto tm = millis() - t;
//	utils::sformat("%3d%% %dms (%d)", buf, sizeof(buf)) % ((y+q)*100/dh) % tm % raysPerPixel;
//	draw_text(8, 0, buf);
  }
  {
	auto tm = millis() - t;
	{
		char buf[50];
		auto tm = millis() - t;
		utils::sformat("%dms (%d)", buf, sizeof(buf)) % tm % raysPerPixel;
		draw_text(8, 0, buf);
	}
	utils::format("Render time: %dms (%d)\n") % tm % raysPerPixel;
  }
}
