#include "Canvas.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_glut.h"
#include "imgui/examples/imgui_impl_opengl2.h"
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
    #include <GL/freeglut.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif


map<int, Color> colorMap;

enum clickType_t {
    canvas_click = 0,
    ui_click
} click_type = canvas_click;

ImVec2 windowSize = ImVec2(1000,700);

int selectedStrokeID;
float theta = 3.14f / 2.f;
int axisID = 1;
Canvas *canvas1 = new Canvas(0, 0, windowSize.x/2, windowSize.y, windowSize.x, windowSize.y);
Canvas *canvas2 = new Canvas(windowSize.x/2, 0, windowSize.x, windowSize.y, windowSize.x, windowSize.y);


void createUI()
{

    int numSwatches = colorMap.size();
    ImVec2 swatchSize = ImVec2(100,30);
    ImVec2 swatchPadding = ImVec2(15,8);
    ImVec2 swatchWindow = ImVec2((swatchSize.x + swatchPadding.x), (swatchSize.y + swatchPadding.y) * numSwatches );

    ImGui::Begin("Swatches");  
    ImGui::SetWindowSize(swatchWindow);

    for(map<int, Color>::iterator stroke = colorMap.begin(); stroke != colorMap.end(); stroke++) {
        const char* name = Canvas::strokeDict[stroke->first].c_str();
        Color c = stroke->second;
        if(selectedStrokeID == stroke->first) {
            c = c.scale(0.5);
        }
        if(ImGui::ColorButton(name, ImVec4(c.r, c.g, c.b, 1.0), ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, swatchSize)) {
            canvas1->swatchSwitch(stroke->first);
            canvas2->swatchSwitch(stroke->first);
            selectedStrokeID = stroke->first;
        }
    }

    ImGui::End();

    ImGui::Begin("Canvas View");  
    ImGui::SetWindowSize(ImVec2(200,100));

    if(ImGui::SliderFloat("rotation", &theta, 0, 3.14, "%.2f")) {
        canvas2->setTransform( Matrix3x3(axisID, theta) );
    }

    const char* axes[] = { "X", "Y", "Z"};
    static const char* axis = "X";

    if (ImGui::BeginCombo("##axes", axis)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(axes); n++)
        {
            bool is_selected = (axis == axes[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(axes[n], is_selected)) {
                axis = axes[n];
                axisID = n;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::End();

}

void drawCanvas() {

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
    glVertex2f(-0.001, 1);
    glVertex2f( 0.001, 1);
    glVertex2f( 0.001, -1);
    glVertex2f(-0.001, -1);
    glEnd();
    
         
}

void glut_display_func()
{
    ImGui::SetNextWindowSize(ImVec2(500,500));

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    createUI();

    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();

    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    
    drawCanvas();
    

    bool mouse_clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    bool mouse_drag = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    bool mouse_ui_region = io.WantCaptureMouse;
    ImVec2 mousePos = io.MousePos;


    if (mouse_clicked) {
        if (mouse_ui_region) {
            click_type = ui_click;
        }
        else {
            click_type = canvas_click;
            if(mousePos.x < io.DisplaySize.x/2) {
                canvas1->mouse(mousePos.x, mousePos.y);
                canvas2->mouse(mousePos.x, mousePos.y);
            }
            else {
                canvas1->mouse(mousePos.x, mousePos.y);
                canvas2->mouse(mousePos.x, mousePos.y);
            }
        }
    }

    if (mouse_drag) {
        if(click_type == canvas_click) {
            if(mousePos.x < io.DisplaySize.x/2){
                canvas1->motion(mousePos.x, mousePos.y);
                canvas2->motion(mousePos.x + io.DisplaySize.x/2, mousePos.y);
            }
            else {
                canvas1->motion(mousePos.x - io.DisplaySize.x/2, mousePos.y);
                canvas2->motion(mousePos.x, mousePos.y);
            }
        }
    }

    canvas1->display();
    canvas2->display(); 

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    
    glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(windowSize.x, windowSize.y);
    glutCreateWindow("Painter");

    colorMap = Canvas::createColors();

    canvas2->setTransform( Matrix3x3(axisID, theta) );

    glutDisplayFunc(glut_display_func);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
