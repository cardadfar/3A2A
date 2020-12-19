from random import random
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.uix.slider import Slider
from kivy.graphics import Color, Ellipse, Line, Rectangle
from kivy.core.window import Window




class MyPaintWidget(Widget):

    points = []
    color = 'black'
    point_start = ()

    wth, hgt = Window.size


    def clear(self):
        with self.canvas:
            Color(*(1,1,1), mode='rgb')
            Rectangle(size=(self.wth, self.hgt))
            Color(*(0,0,0), mode='rgb')
        self.points = []

    def on_touch_down(self, touch):
        if(touch.x > 100):
            with self.canvas:
                Ellipse(size=(20,20), pos=(touch.x-10, touch.y-10))
                touch.ud['line'] = Line(points=(touch.x, touch.y))
                touch.ud['line'].width = 5
            self.point_start = (touch.x, touch.y)

    def on_touch_up(self, touch):
        if(touch.x > 100):
            with self.canvas:
                Ellipse(size=(20,20), pos=(touch.x-10, touch.y-10))
                touch.ud['line'] = Line(points=(touch.x, touch.y))
                touch.ud['line'].width = 5
            self.point_end = (touch.x, touch.y)
            self.points.append((self.color, self.point_start, self.point_end))

    def on_touch_move(self, touch):
        if(touch.x > 100):
            if('line' in touch.ud.keys()):
                touch.ud['line'].points += [touch.x, touch.y]
    






class MyPaintApp(App):

    frameNum = 1
    parent = 0

    def build(self):

        wth, hgt = Window.size

        self.parent = Widget()
        self.painter = MyPaintWidget()
        self.painter.size = [wth,hgt]
        self.parent.add_widget(self.painter)

        self.painter.clear()

        self.add_widgets()

        with self.painter.canvas:
            Color(*(0,0,0), mode='rgb')

        return self.parent


        

    def add_widgets(self):

        wth, hgt = Window.size

        s = Slider(min=0, max=50, value=0)
        s.pos = [100,0]
        s.size = [wth-100,100]
        s.border_horizontal = (1,1,1,1)
        self.parent.add_widget(s)

        # clear button
        clearbtn = Button(text='Clear')
        clearbtn.bind(on_release=self.clear_canvas)
        clearbtn.pos = [0,0]
        self.parent.add_widget(clearbtn)

        # export button
        exportbtn = Button(text='Export')
        exportbtn.bind(on_release=self.export)
        exportbtn.pos = [0,100]
        self.parent.add_widget(exportbtn)

        # red swatch button
        redbtn = Button(text='Head')
        redbtn.bind(on_release=self.color_red)
        redbtn.background_color = [1,0,0,1]
        redbtn.pos = [0,hgt-100]
        self.parent.add_widget(redbtn)

        # green swatch button
        greenbtn = Button(text='Back')
        greenbtn.bind(on_release=self.color_green)
        greenbtn.background_color = [0.5,0.7,0,1]
        greenbtn.pos = [0,hgt-200]
        self.parent.add_widget(greenbtn)

        # blue swatch button
        bluebtn = Button(text='L-Lefg')
        bluebtn.bind(on_release=self.color_blue)
        bluebtn.background_color = [0,0,1,1]
        bluebtn.pos = [0,hgt-300]
        self.parent.add_widget(bluebtn)

        # yellow swatch button
        yellowbtn = Button(text='L-Ankle')
        yellowbtn.bind(on_release=self.color_yellow)
        yellowbtn.background_color = [1,0.8,0.3,1]
        yellowbtn.pos = [0,hgt-400]
        self.parent.add_widget(yellowbtn)

        # pink swatch button
        pinkbtn = Button(text='R-Leg')
        pinkbtn.bind(on_release=self.color_pink)
        pinkbtn.background_color = [1,0.3,0.65,1]
        pinkbtn.pos = [0,hgt-500]
        self.parent.add_widget(pinkbtn)

        # brown swatch button
        brownbtn = Button(text='R-Ankle')
        brownbtn.bind(on_release=self.color_brown)
        brownbtn.background_color = [0.4,0.3,0.1,1]
        brownbtn.pos = [0,hgt-600]
        self.parent.add_widget(brownbtn)

        # cyan swatch button
        cyanbtn = Button(text='R-Shldr')
        cyanbtn.bind(on_release=self.color_cyan)
        cyanbtn.background_color = [0.6,0.9,0.75,1]
        cyanbtn.pos = [0,hgt-700]
        self.parent.add_widget(cyanbtn)

        # purple swatch button
        purplebtn = Button(text='R-Arm')
        purplebtn.bind(on_release=self.color_purple)
        purplebtn.background_color = [0.3,0.1,0.7,1]
        purplebtn.pos = [0,hgt-800]
        self.parent.add_widget(purplebtn)

        # grey swatch button
        greybtn = Button(text='L-Shldr')
        greybtn.bind(on_release=self.color_grey)
        greybtn.background_color = [0.5,0.5,0.5,1]
        greybtn.pos = [0,hgt-900]
        self.parent.add_widget(greybtn)

        # black swatch button
        blackbtn = Button(text='L-Arm')
        blackbtn.bind(on_release=self.color_black)
        blackbtn.background_color = [1,1,1,1]
        blackbtn.pos = [0,hgt-1000]
        self.parent.add_widget(blackbtn)







    def clear_canvas(self, obj):
        self.painter.canvas.clear()
        self.painter.clear()
        with self.painter.canvas:
            Color(*(0,0,0), mode='rgb')

    def export(self, obj):
        self.painter.export_to_png('./output/frame' + str(self.frameNum) + '.png')
        f = open("./output/data" + str(self.frameNum) + ".txt","w+")
        for p in self.painter.points:
            print(p)
            c, (x0,y0), (x1,y1) = p
            f.write("%s %d %d %d %d\n" % (c,x0,y0,x1,y1))
        f.close()
        self.frameNum += 1






    def color_red(self, obj):
        with self.painter.canvas:
            Color(*(1,0,0), mode='rgb')
        self.painter.color = 'red'

    def color_green(self, obj):
        with self.painter.canvas:
            Color(*(0.5,0.7,0), mode='rgb')
        self.painter.color = 'green'

    def color_blue(self, obj):
        with self.painter.canvas:
            Color(*(0,0,1), mode='rgb')
        self.painter.color = 'blue'

    def color_yellow(self, obj):
        with self.painter.canvas:
            Color(*(1,0.8,0.3), mode='rgb')
        self.painter.color = 'yellow'

    def color_pink(self, obj):
        with self.painter.canvas:
            Color(*(1,0.3,0.65), mode='rgb')
        self.painter.color = 'pink'

    def color_brown(self, obj):
        with self.painter.canvas:
            Color(*(0.4,0.3,0.1), mode='rgb')
        self.painter.color = 'brown'

    def color_cyan(self, obj):
        with self.painter.canvas:
            Color(*(0.6,0.9,0.75), mode='rgb')
        self.painter.color = 'cyan'

    def color_purple(self, obj):
        with self.painter.canvas:
            Color(*(0.3,0.1,0.7), mode='rgb')
        self.painter.color = 'purple'

    def color_grey(self, obj):
        with self.painter.canvas:
            Color(*(0.5,0.5,0.5), mode='rgb')
        self.painter.color = 'grey'

    def color_black(self, obj):
        with self.painter.canvas:
            Color(*(0,0,0), mode='rgb')
        self.painter.color = 'black'



if __name__ == '__main__':
    MyPaintApp().run()