"""
To add:
    Inventory grid (have clickable images of items)
    DONE
    self.connect("key-press-event", *handler*)
        if up/W, move up
        if down/S, move down
        ...etc.
        (other actions)
        DONE
    Have map_grid feature thin border around each tile.
        Highlight current tile that player is on.
        DONE
    Functions to add:
        update_map_grid()
        update_description()
        move(current_tile, direction)

    Classes to add:
        Tile (will have Gtk.Image, title (forest/grass/path/etc.),
            method(s) to run when user moves onto that tile, discovered
            boolean (has user seen this tile before? True/False))
            DONE
        Battle (will have methods to run for battle and text to display
            when in battle mode)
        InventoryItem (image, title, item_type, message(?))
            IN PROGRESS
        Health (current_value, endurance, hunger, deplete_health(),
            restore_health())
            IN PROGRESS

"""

import sys
import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk, Gdk, GdkPixbuf, Pango

from Tile import Tile
from InventoryItem import InventoryItem

class GameWindow(Gtk.Window):
    MAP_TILE_SIZE = 40
    MAP_TILE_NUM = 10
    INVENTORY_COLUMNS = 4
    def __init__(self, title = "Game"):
        Gtk.Window.__init__(self)
        self.set_title(title)

        self.x = self.MAP_TILE_NUM / 2
        self.y = self.MAP_TILE_NUM - 1
        self.tiles = [ [0 for x in range(self.MAP_TILE_NUM)]
            for y in range(self.MAP_TILE_NUM) ]
        for y in range(self.MAP_TILE_NUM):
            for x in range(self.MAP_TILE_NUM):
                if ([x, y] == [1, 0] or [x, y] == [2, 0] or [x, y] == [4, 0] or
                    [x, y] == [2, 1] or [x, y] == [4, 2] or [x, y] == [0, 4] or
                    [x, y] == [1, 4]):
                    self.tiles[x][y] = Tile(self, x, y, Tile.TITLE_FOREST)
                elif ([x, y] == [3, 0] or [x, y] == [3, 1] or [x, y] == [3, 2] or
                    [x, y] == [3, 3] or [x, y] == [4, 3] or [x, y] == [4, 4] or
                    [x, y] == [4, 5] or [x, y] == [4, 6] or [x, y] == [4, 7] or
                    [x, y] == [4, 8] or [x, y] == [4, 9]):
                    self.tiles[x][y] = Tile(self, x, y, Tile.TITLE_PATH)
                else:
                    self.tiles[x][y] = Tile(self, x, y, Tile.TITLE_GRASS)

                if abs(x - self.x) <= 1 and abs(y - self.y) <= 1:
                    self.tiles[x][y].set_discovered(Tile.DISCOVERED)
                else:
                    self.tiles[x][y].set_discovered(not Tile.DISCOVERED)
        self.tiles[self.x][self.y].set_highlighted(Tile.HIGHLIGHTED)

        self.layout_grid = Gtk.Grid()

        self.map_grid = Gtk.Grid()
        self.map_grid.set_vexpand(True)
        self.map_grid.set_hexpand(True)
        self.map_grid.set_valign(Gtk.Align.CENTER)
        self.map_grid.set_halign(Gtk.Align.CENTER)
        self.map_grid.set_border_width(2)
        self.map_grid_box = Gtk.Box()
        self.map_grid_box.modify_bg(Gtk.StateType.NORMAL, Tile.REGULAR_COLOR)
        self.map_grid_box.add(self.map_grid)

        self.user_health_bar = Gtk.LevelBar()
        self.user_health_bar.set_min_value(0)
        self.user_health_bar.set_max_value(100)
        self.user_health_bar.set_value(50)
        self.user_health_bar.set_size_request(150, 15)
        self.user_health_bar.set_vexpand(False)
        self.user_health_bar.set_hexpand(False)
        self.user_health_bar.set_valign(Gtk.Align.END)
        self.user_health_bar.set_halign(Gtk.Align.CENTER)
        self.user_health_bar_box = Gtk.Box()
        self.user_health_bar_box.set_border_width(6)
        self.user_health_bar_box.add(self.user_health_bar)

        self.enemy_health_bar = Gtk.LevelBar()
        self.enemy_health_bar.set_min_value(0)
        self.enemy_health_bar.set_max_value(100)
        self.enemy_health_bar.set_value(50)
        self.enemy_health_bar.set_size_request(150, 15)
        self.enemy_health_bar.set_vexpand(False)
        self.enemy_health_bar.set_hexpand(True)
        self.enemy_health_bar.set_valign(Gtk.Align.START)
        self.enemy_health_bar.set_halign(Gtk.Align.CENTER)
        self.enemy_health_bar_box = Gtk.Box()
        self.enemy_health_bar_box.set_border_width(6)
        self.enemy_health_bar_box.add(self.enemy_health_bar)

        self.font = Pango.FontDescription("Dejavu, Sans, Mono, 12")
        self.description = Gtk.Label()
        self.description.set_size_request(0, 50)
        self.description.set_text("Welcome to the game!")
        # self.description.modify_font(self.font)

        self.inventory_button = Gtk.Button("INVENTORY")
        self.inventory_button.set_size_request(150, 1)
        self.inventory_button.connect("clicked", self.on_clicked)
        self.inventory_box = Gtk.Box()
        self.inventory_box.set_border_width(6)
        self.inventory_box.add(self.inventory_button)

        self.inventory_grid = Gtk.Grid()
        self.inventory_items = []
        for i in range(5):
            if i == 0 or i == 1 or i == 3:
                self.inventory_items.append(
                    InventoryItem(self, InventoryItem.TITLE_SWORD))
            else:
                self.inventory_items.append(
                    InventoryItem(self, InventoryItem.TITLE_BEEF))
        for i in range(len(self.inventory_items)):
            self.inventory_grid.attach(self.inventory_items[i],
                i % self.INVENTORY_COLUMNS, i / self.INVENTORY_COLUMNS, 1, 1)

        self.inventory_popover = Gtk.Popover()
        self.inventory_popover.set_relative_to(self.inventory_button)
        self.inventory_popover.set_position(Gtk.PositionType.TOP)
        self.inventory_popover.add(self.inventory_grid)

        for y in range(self.MAP_TILE_NUM):
            for x in range(self.MAP_TILE_NUM):
                self.map_grid.attach(self.tiles[x][y], x, y, 1, 1)

        self.layout_grid.attach(self.enemy_health_bar_box, 1, 0, 1, 1)
        self.layout_grid.attach(self.map_grid_box, 1, 1, 1, 1)
        self.layout_grid.attach(self.user_health_bar_box, 0, 2, 1, 1)
        self.layout_grid.attach(self.description, 1, 2, 1, 2)
        self.layout_grid.attach(self.inventory_box, 2, 2, 1, 1)

        self.add(self.layout_grid)

        self.connect("key-press-event", self.on_key_press_event)

    def on_clicked(self, button):
        self.inventory_popover.popup()
        self.inventory_popover.show_all()

    def on_key_press_event(self, widget, event):
        if (event.keyval == Gdk.KEY_Up):
            sys.stdout.write("UP\n")
            sys.stdout.flush()
            self.move_up()
        elif (event.keyval == Gdk.KEY_Down):
            sys.stdout.write("DOWN\n")
            sys.stdout.flush()
            self.move_down()
        elif (event.keyval == Gdk.KEY_Left):
            sys.stdout.write("LEFT\n")
            sys.stdout.flush()
            self.move_left()
        elif (event.keyval == Gdk.KEY_Right):
            sys.stdout.write("RIGHT\n")
            sys.stdout.flush()
            self.move_right()
        else:
            pass

    def move_up(self):
        if self.y - 1 >= 0 and self.y - 1 < self.MAP_TILE_NUM:
            self.tiles[self.x][self.y].set_highlighted(not Tile.HIGHLIGHTED)
            self.y -= 1
            self.tiles[self.x][self.y].set_highlighted(Tile.HIGHLIGHTED)

            self.description.set_text(self.tiles[self.x][self.y].text)
            for y in range(5):
                for x in range(5):
                    if abs(x - self.x) <= 1 and abs(y - self.y) <= 1:
                        self.tiles[x][y].set_discovered(Tile.DISCOVERED)
                    else:
                        self.tiles[x][y].set_discovered(not Tile.DISCOVERED)
            self.update_map()

    def move_down(self):
        if self.y + 1 >= 0 and self.y + 1 < self.MAP_TILE_NUM:
            self.tiles[self.x][self.y].set_highlighted(not Tile.HIGHLIGHTED)
            self.y += 1
            self.tiles[self.x][self.y].set_highlighted(Tile.HIGHLIGHTED)

            self.description.set_text(self.tiles[self.x][self.y].text)
            for y in range(5):
                for x in range(5):
                    if abs(x - self.x) <= 1 and abs(y - self.y) <= 1:
                        self.tiles[x][y].set_discovered(Tile.DISCOVERED)
                    else:
                        self.tiles[x][y].set_discovered(not Tile.DISCOVERED)
            self.update_map()

    def move_left(self):
        if self.x - 1 >= 0 and self.x - 1 < self.MAP_TILE_NUM:
            self.tiles[self.x][self.y].set_highlighted(not Tile.HIGHLIGHTED)
            self.x -= 1
            self.tiles[self.x][self.y].set_highlighted(Tile.HIGHLIGHTED)

            self.description.set_text(self.tiles[self.x][self.y].text)
            for y in range(5):
                for x in range(5):
                    if abs(x - self.x) <= 1 and abs(y - self.y) <= 1:
                        self.tiles[x][y].set_discovered(Tile.DISCOVERED)
                    else:
                        self.tiles[x][y].set_discovered(not Tile.DISCOVERED)
            self.update_map()

    def move_right(self):
        if self.x + 1 >= 0 and self.x + 1 < self.MAP_TILE_NUM:
            self.tiles[self.x][self.y].set_highlighted(not Tile.HIGHLIGHTED)
            self.x += 1
            self.tiles[self.x][self.y].set_highlighted(Tile.HIGHLIGHTED)

            self.description.set_text(self.tiles[self.x][self.y].text)
            for y in range(5):
                for x in range(5):
                    if abs(x - self.x) <= 1 and abs(y - self.y) <= 1:
                        self.tiles[x][y].set_discovered(Tile.DISCOVERED)
                    else:
                        self.tiles[x][y].set_discovered(not Tile.DISCOVERED)
            self.update_map()

    def update_map(self):
        for y in range(5):
            for x in range(5):
                self.map_grid.remove(self.tiles[x][y])
        for y in range(5):
            for x in range(5):
                self.map_grid.attach(self.tiles[x][y], x, y, 1, 1)

win = GameWindow()
win.show_all()
win.connect("destroy", Gtk.main_quit)
Gtk.main()
