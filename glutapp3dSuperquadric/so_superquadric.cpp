
# include "so_superquadric.h"

SoSuperquadric::SoSuperquadric()
 {
   _numpoints = 0;
   _phong = false;
   _flatn = true;
 }

void SoSuperquadric::init ()
 {
   // Load programs:
   _vshgou.load_and_compile ( GL_VERTEX_SHADER, "../vsh_mcol_gouraud.glsl" );
   _fshgou.load_and_compile ( GL_FRAGMENT_SHADER, "../fsh_gouraud.glsl" );
   _proggouraud.init_and_link ( _vshgou, _fshgou );

   _vshphong.load_and_compile ( GL_VERTEX_SHADER, "../vsh_mcol_phong.glsl" );
   _fshphong.load_and_compile ( GL_FRAGMENT_SHADER, "../fsh_mcol_phong.glsl" );
   _progphong.init_and_link ( _vshphong, _fshphong );

   // Define buffers needed:
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 3 );       // will use 3 buffers

   _proggouraud.uniform_locations ( 9 ); // will send 9 variables
   _proggouraud.uniform_location ( 0, "vTransf" );
   _proggouraud.uniform_location ( 1, "vProj" );
   _proggouraud.uniform_location ( 2, "lPos" );
   _proggouraud.uniform_location ( 3, "la" );
   _proggouraud.uniform_location ( 4, "ld" );
   _proggouraud.uniform_location ( 5, "ls" );
   _proggouraud.uniform_location ( 6, "ka" );
   _proggouraud.uniform_location ( 7, "ks" );
   _proggouraud.uniform_location ( 8, "sh" );

   _progphong.uniform_locations ( 9 ); // will send 9 variables
   _progphong.uniform_location ( 0, "vTransf" );
   _progphong.uniform_location ( 1, "vProj" );
   _progphong.uniform_location ( 2, "lPos" );
   _progphong.uniform_location ( 3, "la" );
   _progphong.uniform_location ( 4, "ld" );
   _progphong.uniform_location ( 5, "ls" );
   _progphong.uniform_location ( 6, "ka" );
   _progphong.uniform_location ( 7, "ks" );
   _progphong.uniform_location ( 8, "sh" );
 }

static float sgn(float x) { return x<0? -1.0f: x>0? 1.0f:0; }
static float cf(float w, float m) { float cw=(float)cos(w); return sgn(cw)*pow(GS_ABS(cw),m); }
static float sf(float w, float m) { float sw=(float)sin(w); return sgn(sw)*pow(GS_ABS(sw),m); }

//See formula in https://en.wikipedia.org/wiki/Superquadrics
static GsPnt evaluate ( float u, float v, float *p )
 {
   const float w=2.0f;
   float r=p[0]; float s=p[1]; float t=p[2]; float A=p[3]; float B=p[4]; float C=p[5];
   return GsPnt (
   A * cf(v,w/r) * cf(u,w/r),
   B * cf(v,w/s) * sf(u,w/s),
   C * sf(v,w/t)
   );
 }

static GsVec estnormal ( float u, float v, float* p, float inc )
 {
   // here we estimate the tangent plane with three points around (u,v)
   // and use the plane normal as the normal at point (u,v)
   return normal (
   evaluate ( u-inc, v-inc, p ),
   evaluate ( u+inc, v-inc, p ),
   evaluate ( u, v+inc, p ) );
 }

void SoSuperquadric::build ( float r, float s, float t, float a, float b, float c )
 {
   /* Here we evaluate the parameteric equation of superquarics.
      This can be optimized in many ways but is ok for this example project.
   */

   float parms[6];
   parms[0]=r; parms[1]=s; parms[2]=t; parms[3]=a; parms[4]=b; parms[5]=b;

   // build arrays:

   P.size(0); C.size(0); N.size(0); NL.size(0);
   GsVec n1, n2, n3, n4;

   float inc = gspi/14.0f; // The tesselation resolution is given here
   float deltainc = inc/4.0f;
   for ( float u=-gspi+inc; u<gspi+deltainc; u+=inc )
    {
      for ( float v=-gspidiv2+inc; v<gspidiv2; v+=inc )
       { GsPnt a = evaluate ( u-inc, v-inc, parms );
         GsPnt b = evaluate ( u-inc, v, parms );
         GsPnt c = evaluate ( u, v, parms );
         GsPnt d = evaluate ( u, v-inc, parms );
         P.push()=a; P.push()=c; P.push()=b; 
         P.push()=a; P.push()=d; P.push()=c; 

         if ( _flatn )
          { n1 = normal (a,c,b);
            n2 = normal (a,d,c); 
            if ( v<-gspidiv2+inc+deltainc ) n2*=-1; // this is to fix the normals at the limit of parameterization
            N.push()=n1; N.push()=n1; N.push()=n1;
            N.push()=n2; N.push()=n2; N.push()=n2;
          }
         else
          { n1 = estnormal ( u-inc, v-inc, parms, deltainc );
            n2 = estnormal ( u-inc, v, parms, deltainc );
            n3 = estnormal ( u, v, parms, deltainc );
            n4 = estnormal ( u, v-inc, parms, deltainc );
            if ( v<=-gspidiv2+inc+deltainc ) {n1*=-1; n4*=-1; } // normals quick fix
            N.push()=n1; N.push()=n3; N.push()=n2;
            N.push()=n1; N.push()=n4; N.push()=n3;
          }

         // build NL for later normal visualization with SoLines:
         const float ns=0.05f; // how long they appear in the screen
         if ( _flatn )
          { n1*=ns; n2*=ns;
            GsPnt f1c=(a+c+b)/3.0f; NL.push()=f1c; NL.push()=f1c+n1;
            GsPnt f2c=(a+d+c)/3.0f; NL.push()=f2c; NL.push()=f2c+n2;
          }
         else
          { n1*=ns; n2*=ns; n3*=ns; n4*=ns;
            NL.push()=a; NL.push()=a+n1; NL.push()=b; NL.push()=b+n2;
          }
       }
    }

   /* There are multiple ways to organize data to send to OpenGL. 
      Here we send material information in uniforms and we only send the diffuse color
      information per vertex. The other components are uniforms sent to affect the whole object.
      This is a solution that keeps this code simple and is ok for most objects.
   */

   C.size(P.size()); C.setall(GsColor::red);
   _mtl.specular.set ( 255,255,255 );
   _mtl.shininess = 8; // increase specular effect

   // send data to OpenGL buffers:
   glBindVertexArray ( va[0] );
   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );
   glEnableVertexAttribArray ( 2 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, 3*sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
   glBufferData ( GL_ARRAY_BUFFER, 3*sizeof(float)*N.size(), N.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[2] );
   glBufferData ( GL_ARRAY_BUFFER, 4*sizeof(gsbyte)*C.size(), C.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glBindVertexArray(0); // break the existing vertex array object binding.

   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numpoints = P.size();

   // on first build only print number of triangles to keep an eye on the overall resolution:
   static bool firstbuild=true;
   if ( firstbuild ) { std::cout<<"\nNum triangles in superquadric: "<<_numpoints/3.0f<<"\n\n"; firstbuild=false; }

   // free non-needed memory:
   P.capacity(0); C.capacity(0); N.capacity(0);
   // (leave NL untouched because it will be used by another class for display)
 }

void SoSuperquadric::draw ( const GsMat& tr, const GsMat& pr, const GsLight& l )
 {
   float f[4]; 
   float sh = (float)_mtl.shininess;
   if ( sh<0.001f ) sh=64;

   if ( _phong )
    { glUseProgram ( _progphong.id );
      glUniformMatrix4fv ( _progphong.uniloc[0], 1, GL_FALSE, tr.e );
      glUniformMatrix4fv ( _progphong.uniloc[1], 1, GL_FALSE, pr.e );
      glUniform3fv ( _progphong.uniloc[2], 1, l.pos.e );
      glUniform4fv ( _progphong.uniloc[3], 1, l.amb.get(f) );
      glUniform4fv ( _progphong.uniloc[4], 1, l.dif.get(f) );
      glUniform4fv ( _progphong.uniloc[5], 1, l.spe.get(f) );
      glUniform4fv ( _progphong.uniloc[6], 1, _mtl.ambient.get(f) );
      glUniform4fv ( _progphong.uniloc[7], 1, _mtl.specular.get(f) );
      glUniform1fv ( _progphong.uniloc[8], 1, &sh );
    }
   else
    { glUseProgram ( _proggouraud.id );
      glUniformMatrix4fv ( _proggouraud.uniloc[0], 1, GL_FALSE, tr.e );
      glUniformMatrix4fv ( _proggouraud.uniloc[1], 1, GL_FALSE, pr.e );
      glUniform3fv ( _proggouraud.uniloc[2], 1, l.pos.e );
      glUniform4fv ( _proggouraud.uniloc[3], 1, l.amb.get(f) );
      glUniform4fv ( _proggouraud.uniloc[4], 1, l.dif.get(f) );
      glUniform4fv ( _proggouraud.uniloc[5], 1, l.spe.get(f) );
      glUniform4fv ( _proggouraud.uniloc[6], 1, _mtl.ambient.get(f) );
      glUniform4fv ( _proggouraud.uniloc[7], 1, _mtl.specular.get(f) );
      glUniform1fv ( _proggouraud.uniloc[8], 1, &sh );
    }

   glBindVertexArray ( va[0] );
   glDrawArrays ( GL_TRIANGLES, 0, _numpoints );
   glBindVertexArray(0); // break the existing vertex array object binding.
 }

