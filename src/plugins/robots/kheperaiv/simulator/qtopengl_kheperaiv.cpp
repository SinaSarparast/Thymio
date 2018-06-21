/**
 * @file <argos3/plugins/robots/kheperaiv/simulator/qtopengl_kheperaiv.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_kheperaiv.h"
#include "kheperaiv_entity.h"
#include "kheperaiv_measures.h"

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <QImage>

namespace argos {

  static const Real HALF_CHASSIS_LENGHT = 0.056f;
  static const Real HALF_CHASSIS_WIDTH  = 0.055f;
  static const Real BODY_HEIGHT         = 0.053f; 
   /****************************************/
   /****************************************/

   // static QOpenGLTexture* MakeTexture(const QString& str_fname) {
   //    QString strPath =
   //       QString(ARGOS_INSTALL_PREFIX) +
   //       "/include/argos3/plugins/simulator/visualizations/qt-opengl/textures/" +
   //       str_fname;
   //    return new QOpenGLTexture(QImage(strPath));
   // }

   /****************************************/
   /****************************************/


   CQTOpenGLKheperaIV::CQTOpenGLKheperaIV() :
      m_unVertices(40) {
      /* Reserve the needed textures */
      // m_pcTextures[0] = MakeTexture("kheperaiv_texture_top.png");
      // m_pcTextures[1] = MakeTexture("kheperaiv_texture_bottom.png");
      // m_pcTextures[2] = MakeTexture("kheperaiv_texture_side.png");
      /* Reserve the needed display lists */
      m_unLists = glGenLists(3);
      /* Assign indices for better referencing (later) */

      m_unWheelList = m_unLists;
      m_unBaseList  = m_unLists + 1;
      m_unLEDList   = m_unLists + 2;

      /* Create the body display list */
      glNewList(m_unBaseList, GL_COMPILE);
      RenderBody();
      glEndList();


      /* Create the LED display list */
      // glNewList(m_unLEDList, GL_COMPILE);
      // RenderLED();
      // glEndList();
      /* Create the wheel display list */
      glNewList(m_unWheelList, GL_COMPILE);
      RenderWheel();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLKheperaIV::~CQTOpenGLKheperaIV() {
      glDeleteLists(m_unLists, 2);
      // delete m_pcTextures[0];
      // delete m_pcTextures[1];
      // delete m_pcTextures[2];
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKheperaIV::Draw(CKheperaIVEntity& c_entity) {
      /* Draw the body */
      glPushMatrix();
      // glScalef(THYMIO_LENGHT, THYMIO_WIDTH, THYMIO_HEIGHT);
      /* Place the body */
      glCallList(m_unBaseList);
      glPopMatrix();
            
      /* Place the LEDs */
      CLEDEquippedEntity& cLEDEquippedEntity = c_entity.GetLEDEquippedEntity();
      for(UInt32 i = 0; i < 3; i++) {
         const CColor& cColor = cLEDEquippedEntity.GetLED(i).GetColor();
         const CVector3& cOffset = cLEDEquippedEntity.GetLEDOffset(i);
         SetLEDMaterial(cColor.GetRed(), cColor.GetGreen(), cColor.GetBlue());
         glPushMatrix();
         glTranslatef(cOffset.GetX(), cOffset.GetY(), cOffset.GetZ());
         glCallList(m_unLEDList);
         glPopMatrix();
      }

      // m_pcEmbodiedEntity& cEmbodiedEntity = c_entity.GetEmbodiedEntity();
      

      /* Place the wheels */
      glPushMatrix();
      glTranslatef(0.0f, INTERWHEEL_DISTANCE*0.5f   , 0.0f);
      glCallList(m_unWheelList);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(0.0f, -INTERWHEEL_DISTANCE*0.5f  , 0.0f);
      glCallList(m_unWheelList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKheperaIV::SetLEDMaterial(GLfloat f_red, GLfloat f_green, GLfloat f_blue) {
      const GLfloat pfColor[]     = { f_red, f_green, f_blue, 1.0f };
      const GLfloat pfSpecular[]  = {  0.0f,    0.0f,   0.0f, 1.0f };
      const GLfloat pfShininess[] = {  0.0f                        };
      const GLfloat pfEmission[]  = { f_red, f_green, f_blue, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/
   void CQTOpenGLKheperaIV::SetWhitePlasticMaterial() {
      const GLfloat pfColor[]     = {   1.0f, 1.0f, 1.0f, 1.0f };
      const GLfloat pfSpecular[]  = {   0.9f, 0.9f, 0.9f, 1.0f };
      const GLfloat pfShininess[] = { 100.0f                   };
      const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   void CQTOpenGLKheperaIV::RenderBody() {
    CQTOpenGLUserFunctions* qlfunc = new CQTOpenGLUserFunctions();
      qlfunc->DrawBox(
            CVector3(0,0,BODY_HEIGHT/2+THYMIO_BASE_ELEVATION), //position
            CQuaternion( ), //orientation
            CVector3(THYMIO_LENGHT,THYMIO_WIDTH,THYMIO_HEIGHT),//size
            CColor::RED
            );
    // CQTOpenGLUserFunctions::DrawBox(
    //   CVector3(0,0,0), //position
    //   CQuaternion( CVector3(0,0,0),CVector3(0,0,0)), //orientation
    //   CVector3(THYMIO_LENGHT,THYMIO_WIDTH,THYMIO_HEIGHT),//size
    //   CColor::WHITE
    //   )


      // glEnable(GL_NORMALIZE);

      // SetWhitePlasticMaterial();
      // glBegin(GL_QUADS);
      //   /* Bottom face */
      //   glNormal3f( 0.0f,  0.0f,-1.0f);
      //   glVertex3f( HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f( HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f(-HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f(-HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, 0.0f);
        

      //   /* Top face */
      //   glNormal3f( 0.0f,  0.0f, 1.0f);
      //   glVertex3f(-HALF_CHASSIS_WIDTH, -HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   glVertex3f( HALF_CHASSIS_WIDTH, -HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   glVertex3f( HALF_CHASSIS_WIDTH,  HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   glVertex3f(-HALF_CHASSIS_WIDTH,  HALF_CHASSIS_WIDTH, BODY_HEIGHT);


      // glEnd();

      // glBegin(GL_QUADS);
      //   /* South face */
      //   glNormal3f( 0.0f, -1.0f, 0.0f);
      //   glVertex3f(-HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   glVertex3f(-HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f( HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f( HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, BODY_HEIGHT);
        
      //   /* East face */
      //   glNormal3f( 1.0f,  0.0f, 0.0f);
      //   glVertex3f( HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   glVertex3f( HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f( HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f( HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   /* North face */
      //   glNormal3f( 0.0f,  1.0f, 0.0f);
      //   glVertex3f( HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   glVertex3f( HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f(-HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f(-HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   /* West face */
      //   glNormal3f(-1.0f,  0.0f, 0.0f);
      //   glVertex3f(-HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      //   glVertex3f(-HALF_CHASSIS_LENGHT,  HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f(-HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, 0.0f);
      //   glVertex3f(-HALF_CHASSIS_LENGHT, -HALF_CHASSIS_WIDTH, BODY_HEIGHT);
      // glEnd();

      // glDisable(GL_NORMALIZE);

      // glDisable(GL_TEXTURE_2D);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKheperaIV::RenderLED() {
      /* Draw it as a tiny pyramid pointing to Z upwards
         (with no base, cause it's not visible anyway) */
      glBegin(GL_TRIANGLES);
      /* North */
      glVertex3f( 0.000,  0.000, 0.015);
      glVertex3f( 0.015, -0.015, 0.000);
      glVertex3f( 0.015,  0.015, 0.000);
      /* South */
      glVertex3f( 0.000,  0.000, 0.015);
      glVertex3f(-0.015,  0.015, 0.000);
      glVertex3f(-0.015, -0.015, 0.000);
      /* West */
      glVertex3f( 0.000,  0.000, 0.015);
      glVertex3f( 0.015,  0.015, 0.000);
      glVertex3f(-0.015,  0.015, 0.000);
      /* East */
      glVertex3f( 0.000,  0.000, 0.015);
      glVertex3f(-0.015, -0.015, 0.000);
      glVertex3f( 0.015, -0.015, 0.000);
      glEnd();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawKheperaIVNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CKheperaIVEntity& c_entity) {
         static CQTOpenGLKheperaIV m_cModel;
         c_visualization.DrawRays(c_entity.GetControllableEntity());
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawKheperaIVSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CKheperaIVEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   void CQTOpenGLKheperaIV::RenderWheel() {
      /* Set material */
      SetWhitePlasticMaterial();
      /* Right side */
      CVector2 cVertex(WHEEL_RADIUS, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      CVector3 cNormal(-1.0f, -1.0f, 0.0f);
      cNormal.Normalize();
      glBegin(GL_POLYGON);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), -WHEEL_WIDTH*0.5, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();

      /* Left side */
      cVertex.Set(WHEEL_RADIUS, 0.0f);
      cNormal.Set(-1.0f, 1.0f, 0.0f);
      cNormal.Normalize();
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), WHEEL_WIDTH*0.5f, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();

      /* Tire */
      cNormal.Set(1.0f, 0.0f, 0.0f);
      cVertex.Set(WHEEL_RADIUS, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), -WHEEL_WIDTH*0.5f, WHEEL_RADIUS + cVertex.GetY());
         glVertex3f(cVertex.GetX(),  WHEEL_WIDTH*0.5f, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
   }

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawKheperaIVNormal, CKheperaIVEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawKheperaIVSelected, CKheperaIVEntity);

   /****************************************/
   /****************************************/

}