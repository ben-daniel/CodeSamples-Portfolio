import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk, Gdk, GdkPixbuf

class Tile(Gtk.Box):
    """
    Explanation
    """
    SCALE = 40
    TITLE_GRASS = "grass"
    TITLE_FOREST = "forest"
    TITLE_PATH = "path"
    DISCOVERED = True
    HIGHLIGHTED = True
    HIGHLIGHT_COLOR = Gdk.Color.from_floats(
        100 / 255.0,
        200 / 255.0,
        250 / 255.0)
    REGULAR_COLOR = Gdk.Color.from_floats(
        140 / 255.0,
        170 / 255.0,
        110 / 255.0)

    def __init__(self, parent, x, y, title = "grass", discovered = True,
            highlighted = False):
        Gtk.Box.__init__(self)

        self.parent = parent

        self.x = x
        self.y = y
        self.title = title
        self.discovered = discovered

        self.question_image = Gtk.Image.new_from_pixbuf(
            GdkPixbuf.Pixbuf.new_from_file_at_scale(
                "images/question_2.png", Tile.SCALE,
                -1, True))

        if self.title == Tile.TITLE_GRASS:
            self.image_path = "images/grass_2.png"
            self.text = "You are in the grassy field."
        elif self.title == Tile.TITLE_FOREST:
            self.image_path = "images/forest_2.png"
            self.text = "You are in the dark, shadowy forest..."
        elif self.title == Tile.TITLE_PATH:
            self.image_path = "images/path_2.png"
            self.text = "You are safe on the path."
        else:
            self.image_path = "images/grass_1.png"

        self.image = Gtk.Image.new_from_pixbuf(
            GdkPixbuf.Pixbuf.new_from_file_at_scale(
                self.image_path, Tile.SCALE,
                -1, True))

        self.highlighted = highlighted

        self.inner_box = Gtk.Box()

        if self.highlighted:
            self.modify_bg(Gtk.StateType.NORMAL, self.HIGHLIGHT_COLOR)
        else:
            self.modify_bg(Gtk.StateType.NORMAL, self.REGULAR_COLOR)
        self.inner_box.set_border_width(2)

        if self.discovered:
            self.inner_box.add(self.image)
        else:
            self.inner_box.add(self.question_image)
        self.add(self.inner_box)

    def get_discovered(self):
        return self.discovered

    def get_highlighted(self):
        return self.highlighted

    def get_image(self):
        return self.image

    def get_title(self):
        return self.title

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y

    def set_discovered(self, new_discovered):
        self.discovered = new_discovered
        if self.discovered:
            self.inner_box.remove(self.inner_box.get_children()[0])
            # self.inner_box.remove(self.question_image)
            self.inner_box.add(self.image)
        else:
            self.inner_box.remove(self.inner_box.get_children()[0])
            self.inner_box.add(self.question_image)

    def set_highlighted(self, new_highlighted):
        self.highlighted = new_highlighted
        if self.highlighted:
            self.modify_bg(Gtk.StateType.NORMAL, self.HIGHLIGHT_COLOR)
        else:
            self.modify_bg(Gtk.StateType.NORMAL, self.REGULAR_COLOR)

    def set_image(self):
        return self.image

    def set_title(self):
        return self.title

    @classmethod
    def get_regular_color(self):
        return Tile.REGULAR_COLOR

    @classmethod
    def get_highlight_color(self):
        return Tile.HIGHLIGHT_COLOR

    @classmethod
    def set_regular_color(self, new_regular_color):
        Tile.REGULAR_COLOR = new_regular_color

    @classmethod
    def set_highlight_color(self, new_highlight_color):
        Tile.HIGHLIGHT_COLOR = new_highlight_color
