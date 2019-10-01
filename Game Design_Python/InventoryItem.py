import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk, Gdk, GdkPixbuf

class InventoryItem(Gtk.Box):
    SCALE = 40
    TITLE_SWORD = "sword"
    TITLE_BEEF = "beef"
    REGULAR_COLOR = Gdk.Color.from_floats(
        232 / 255.0,
        232 / 255.0,
        232 / 255.0)
    def __init__(self, parent, title):
        Gtk.Box.__init__(self)

        # self.modify_bg(Gtk.StateType.NORMAL, self.REGULAR_COLOR)

        self.parent = parent
        self.title = title

        if self.title == InventoryItem.TITLE_SWORD:
            self.image_path = "images/sword_1.png"
        elif self.title == InventoryItem.TITLE_BEEF:
            self.image_path = "images/beef_2.png"

        self.image = Gtk.Image.new_from_pixbuf(
            GdkPixbuf.Pixbuf.new_from_file_at_scale(
                self.image_path, InventoryItem.SCALE,
                -1, True))
        self.button = Gtk.Button(image = self.image)

        # self.inner_box = Gtk.Box()
        # self.inner_box.set_border_width(1)
        # self.inner_box.add(self.button)
        self.add(self.button)

    def get_image(self):
        return self.image

    def set_image(self, new_image):
        self.image = new_image
