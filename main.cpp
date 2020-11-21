#include <iostream>
#include <fstream>

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

using namespace std;

static float d = 1.0;           // Intensidade da cor difusa da luz branca
static float e = 1.0;           // Intensidade da cor especular da luz branca
static float m = 0.2;           // Intensidade da luz ambiente global
static float p = 1.0;           // A luz branca é posicional?
static float s = 50.0;          // Expoente especular do material (shininess)
static float distancia = 20;
float matShine[] = { s };                       // expoente especular (shininess)
static float xAngle = 0.0, yAngle = 0.0;        // Rotação da luz branca
static long font = (long)GLUT_BITMAP_8_BY_13;   // Fonte usada para imprimir na tela
static char theStringBuffer[10];                // String buffer
static float xMouse = 250, yMouse = 250;        // (x,y) do ponteiro do mouse
static float larguraJanela, alturaJanela;       // (w,h) da janela
static bool isLightingOn = false;               // O sistema de iluminação está ligado?
static float anguloEsferaY = 0;                 // Rotação da esfera em torno do eixo y
static int esferaLados = 200;                   // Quantas subdivisões latitudinais/longitudinais da esfera
static bool localViewer = false;
static bool usarTextura = true;

static int solTexture;
static int mercurioTexture;
static int venusTexture;
static int terraTexture;
static int marteTexture;
static int jupiterTexture;
static int saturnoTexture;
static int uranoTexture;
static int netunoTexture;
//static int plutaoTexture;
static int teste = 10;

float olhoX = 12;
float olhoY =12;
float olhoZ = 5;
int modo = 1;


float diametroSol       =   70/teste;
float diametroMerc      =   9/teste; //1/teste
float diametroVenus     =   15/teste;
float diametroTerra     =   16.2/teste;
float diametroMarte     =   8.4/teste;
float diametroJupiter   =   36/teste;
float diametroSaturno   =   30/teste;
float diametroUrano     =   10.7/teste;
float diametroNetuno    =   10.3/teste;


float distMerc      =   5;
float distVenus     =   10;
float distTerra     =   15;
float distMarte     =   20;
float distJupiter   =   30;
float distSaturno   =   40;
float distUrano     =   50;
float distNetuno    =   60;
float distPlutao    =   70;


float velTransMerc = 0.5;
float velTransVenus = 0.3;
float velTransTerra = 0.1;
float velTransMarte = 0.09;
float velTransJupiter = 0.025;
float velTransSaturno = 0.05;
float velTransUrano = 0.03;
float velTransNetuno = 0.01;


/* 
    Distância média Planetas-Sol
    Planeta     Distancia KM            1cm = 10 milhões km
    Mercúrio    57.910.000      ou      5.8
    Vênus       109.200.000     ou      10.8
    Terra       149.600.000     ou      15
    Marte       227.940.000     ou      23
    Júpiter     414.000.000     ou      41
    Saturno     1.429.300.000   ou      143
    Urano       2.870.990.000   ou      287
    Netuno      4.505.300.000   ou      450
    Plutão      5.922.000.000   ou      592

    Planeta             Diâmetro equatorial(km)             Diâmetro do astro sendo Júpiter igual a 30 cm(cm)
    Sol                 1.390.000                           291 
    Mercúrio            4.879,4                             1 
    Vênus               12.103,6                            2,5 
    Terra               12.756,2                            2,7 
    Marte               6.794,4                             1,4 
    Júpiter             142.984                             30
    Saturno             120.536                             25
    Urano               51.118                              10,7 
    Netuno              49.538                              10,3
    Plutão              2.320                               0,5 
    Lua                 3.476                               0,7

    Dados: planetario.ufsc.br/o-sistema-solar/
*/


// Configuração inicial do OpenGL e GLUT
void setup(void)
{
    //glClearColor(.4,.4,.4, 0.0);                    // fundo cinza
    glClearColor(0,0,0, 0.0);
    glEnable(GL_DEPTH_TEST);                        // Ativa teste Z

    // Propriedades do material da esfera
    float matAmbAndDif[] = {1.0, 1.0, 1.0, 1.0};    // cor ambiente e difusa: branca
    float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };       // cor especular: branca

    // Definindo as propriedades do material
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Carrega a textura do sol
    solTexture = SOIL_load_OGL_texture
    (
        "sol.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(solTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Carrega a textura de mercurio
    mercurioTexture = SOIL_load_OGL_texture
    (
        "mercurio.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(mercurioTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Carrega a textura de venus
    venusTexture = SOIL_load_OGL_texture
    (
        "venus.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(venusTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Carrega a textura da terra
    terraTexture = SOIL_load_OGL_texture
    (
        "terra.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(terraTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Carrega a textura de marte
    marteTexture = SOIL_load_OGL_texture
    (
        "marte.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(marteTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Carrega a textura de jupiter
    jupiterTexture = SOIL_load_OGL_texture
    (
        "jupiter.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(jupiterTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Carrega a textura de saturno
    saturnoTexture = SOIL_load_OGL_texture
    (
        "saturno.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(saturnoTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Carrega a textura de urano
    uranoTexture = SOIL_load_OGL_texture
    (
        "urano.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(uranoTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Carrega a textura de netuno
    netunoTexture = SOIL_load_OGL_texture
    (
        "netuno.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(netunoTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }


    // Não mostrar faces do lado de dentro
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Esconder o ponteiro do mouse quando dentro da janela
    glutSetCursor(GLUT_CURSOR_NONE);
}

// Desenha uma esfera na origem, com certo raio e subdivisões
// latitudinais e longitudinais
//
// Não podemos usar glutSolidSphere, porque ela não chama
// glTexCoord para os vértices. Logo, se você quer texturizar
// uma esfera, não pode usar glutSolidSphere
void solidSphere(float radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    // chama 01 glNormal para cada vértice.. poderia ser
    // GLU_FLAT (01 por face) ou GLU_NONE
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    // chama 01 glTexCoord por vértice
    gluQuadricTexture(quadObj, GL_TRUE);
    // cria os vértices de uma esfera
    gluSphere(quadObj, radius, stacks, columns);
    // limpa as variáveis que a GLU usou para criar
    // a esfera
    gluDeleteQuadric(quadObj);
}

/*
glTranslatef(-cos(anguloEsferaY*velocidadeTranslacao[i])*distanciaX[i],0,-sin(anguloEsferaY*velocidadeTranslacao[i])*distanciaZ[i]);//movimento de translação
glRotatef(anguloPlaneta, 0, 1, 0); //movimento de rotação
*/

void desenhaPlanetas(){
    // Desenha o sol
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, solTexture);
    }
    glPushMatrix();
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroSol, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    //Desenha mercurio
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, mercurioTexture);
    }
    glPushMatrix();
        glTranslatef(-cos(anguloEsferaY*velTransMerc)*distMerc ,0,-sin(anguloEsferaY*velTransMerc)*distMerc);//movimento de translação
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroMerc, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    //Desenha venus
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, venusTexture);
    }
    glPushMatrix();
        glTranslatef(-cos(anguloEsferaY*velTransVenus)*distVenus ,0,-sin(anguloEsferaY*velTransVenus)*distVenus);//movimento de translação
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroVenus, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    //Desenha terra
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, terraTexture);
    }
    glPushMatrix();
        glTranslatef(-cos(anguloEsferaY*velTransTerra)*distTerra ,0,-sin(anguloEsferaY*velTransTerra)*distTerra);//movimento de translação
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroTerra, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    // Desenha marte
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, marteTexture);
    }
    glPushMatrix();
        glTranslatef(-cos(anguloEsferaY*velTransMarte)*distMarte ,0,-sin(anguloEsferaY*velTransMarte)*distMarte);//movimento de translação
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroMarte, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    // Desenha jupiter
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, jupiterTexture);
    }
    glPushMatrix();
        glTranslatef(-cos(anguloEsferaY*velTransJupiter)*distJupiter ,0,-sin(anguloEsferaY*velTransJupiter)*distJupiter);//movimento de translação
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroJupiter, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    // Desenha saturno
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, saturnoTexture);
    }
    glPushMatrix();
        glTranslatef(-cos(anguloEsferaY*velTransSaturno)*distSaturno ,0,-sin(anguloEsferaY*velTransSaturno)*distSaturno);//movimento de translação
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroSaturno, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    // Desenha urano
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, uranoTexture);
    }
    glPushMatrix();
        glTranslatef(-cos(anguloEsferaY*velTransUrano)*distUrano ,0,-sin(anguloEsferaY*velTransUrano)*distUrano);//movimento de translação
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroUrano, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    // Desenha netuno
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, netunoTexture);
    }
    glPushMatrix();
        glTranslatef(-cos(anguloEsferaY*velTransNetuno)*distNetuno ,0,-sin(anguloEsferaY*velTransNetuno)*distNetuno);//movimento de translação
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(diametroNetuno, esferaLados, esferaLados);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }


    glutSwapBuffers();
}

// Callback de desenho
void desenhaCena()
{
    // Propriedades das fontes de luz
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDif0[] = { d, d, d, 1.0 };
    float lightSpec0[] = { e, e, e, 1.0 };
    float lightPos0[] = { 0.0, 0.0, 3.0, p };
    float lightDifAndSpec1[] = { 0.0, 1.0, 0.0, 1.0 };
    float lightPos1[] = { 1.0, 2.0, 0.0, 1.0 };
    float globAmb[] = { m, m, m, 1.0 };


    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);        // Luz ambiente global
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);// Enable local viewpoint


    // Limpa a tela e o z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Posiciona a câmera de acordo com posição x,y do mouse na janela
    gluLookAt(olhoX, olhoY, olhoZ,
              0, 0, 0,
              0, 1, 0);

    // Desabilita iluminação para desenhar as esferas que representam as luzes
    glDisable(GL_LIGHTING);



    if (isLightingOn) {
        glEnable(GL_LIGHTING);
    }


    // Define (atualiza) o valor do expoente de especularidade
    matShine[0] = s;
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
    glColor3f(1, 1, 1);

    //essa função desenha os planetas
    desenhaPlanetas();
}


// Callback do evento de teclado
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'p':
    case 'P':
        if (p) p = 0.0;
        else p = 1.0;
        break;
    case 'd':
        if (d > 0.0) d -= 0.05;
        break;
    case 'D':
        if (d < 1.0) d += 0.05;
        break;
    case 'e':
        if (e > 0.0) e -= 0.05;
        break;
    case 'E':
        if (e < 1.0) e += 0.05;
        break;
    case 's':
        if (s > 5.0) s -= 2.00;
        break;
    case 'S':
        if (s < 100.0) s += 2.00;
        break;
    case 'm':
        if (m > 0.0) m -= 0.05;
        break;
    case 'M':
        if (m < 1.0) m += 0.05;
        break;
    case 'l':
    case 'L':
        isLightingOn = !isLightingOn;
        break;
    case 't':
    case 'T':
        usarTextura = !usarTextura;
        break;
    case 'v':
    case 'V':
        localViewer = !localViewer;
        break;
    case 'c':
    case 'C':
        if(modo==1){
            olhoX = 12;
            olhoY = 12;
            olhoZ = 5;
            modo*=-1;
        }
        else if(modo==-1){
            olhoX = 10;
            olhoY = 50;
            olhoZ = 0;
            modo*=-1;
            printf("%d\n", modo);
        }

        break;
    default:
        break;
    }
    glutPostRedisplay();
}

// Callback para setas do teclado
void specialKeyInput(int key, int x, int y)
{
    if(key == GLUT_KEY_DOWN)
    {
        xAngle++;
        if (xAngle > 360.0) xAngle -= 360.0;
    }
    if(key == GLUT_KEY_UP)
    {
        xAngle--;
        if (xAngle < 0.0) xAngle += 360.0;
    }
    if(key == GLUT_KEY_RIGHT)
    {
        yAngle++;
        if (yAngle > 360.0) yAngle -= 360.0;
    }
    if(key == GLUT_KEY_LEFT)
    {
        yAngle--;
        if (yAngle < 0.0) yAngle += 360.0;
    }
    glutPostRedisplay();
}

// Callback de redimensionamento
void resize(int w, int h)
{
    larguraJanela = w;
    alturaJanela = h;

    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (float)w/(float)h, 1.0, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void posicionaCamera(int x, int y) {
    xMouse = olhoX; //Quando mexe o mouse a camera muda
    yMouse = olhoY;

    glutPostRedisplay();
}

void rotacionaEsfera() {
    anguloEsferaY += .1f;
    glutPostRedisplay();
}

// Imprime a ajuda no console



int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Teste TP2");
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutPassiveMotionFunc(posicionaCamera);
    glutSpecialFunc(specialKeyInput);
    glutIdleFunc(rotacionaEsfera);
    glEnable(GL_TEXTURE_2D);
    setup();

    glutMainLoop();
}
