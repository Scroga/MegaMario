# Assets Specification
- Entities in the game will be rendered using various Textures and Animations which we will be calling Assets (along with Fonts)
- Assets are loaded once at the beginning of the program and stored in the Assets class, which is stored by the GameEngine class.

## Note:
- All entity positions denote the center of their rectangular sprite. 
  It also denotes the center of the bounding box, if it has one.
  This is set via sprite.setOrigin() in the Animation class constructor. 

## Assets File Specification

- There will be three different line types in the Assets file, each of which correspond to a different type of Asset. They are as follows:

### Texture Asset Specification:
Texture `N P`
- Texture name    `N`    std::string (it will have no spaces)
- Texture path    `P`    std::string (it will have no spaces)

### Animation Asset Specification:
Animation `N T F S`
- Animation name  `N`    std::string (it will have no spaces)
- Texture name    `T`    std::string (refers to an existing texture)
- Frame Count     `F`    int (number of frames in the Animation)
- Anim Speed      `S`    int (number of game frames between anim frames)

### Font Asset Specification:
Font `N P`
- Font name       `N`    std::string (it will have no spaces)
- Font path       `P`    std::string (it will have no spaces)

### Sound Asset Specification:
Sound 
- // TODO:

# Mics:
- The `P` key should pause the game
- Pressing the `T` key toggles drawing textures
- Pressing the `C` key toggles drawing bounding boxes of entities
- Pressing the `G` key toggles drawing of the grid (this should be very helpful for debugging)
- The `ESC` key should go back to the Main Menu, or quit if on the Main Menu

# Level

- Game levels will be specified by a level file, which will contain a list of entity specifications, one per line. It will also contain a single line which specifies properties of the player in that level. The syntax of the lines of the level file are as follows:

## Note:
- All (GX, GY) positions given in the level specification file are given in grid coordinates. The grid cells are of size 64 by 64 pixels, and the entity should be positioned such that the bottom left corner of its texture is aligned with the bottom left corner of the given grid coordinate. The grid starts at (0, 0) in the bottom-left of the screen, and can be seen by pressing the `G` key while the game is running.

## Level config files

### Title Entity Specification:
Tile `N GX GY`
- Animation name   `N`     std::string (Animation asset name for this tile)
- GX Grid X Pos    `GX`    float
- GY Grid Y Pos    `GY`    float

### Decoration Entity Specification:
Dec `N X Y`
- Animation name    `N`    std::string (Animation asset name for this tile)
- X, Y position     `X, Y` float, float 

### Player Specification:
Player `GX GY GW GH SX SY SM GY B`
- GX, GY Grid X,Y Pos `GX, GY` float, float (starting position of player)
- BoundingBox W/H `CW, CH` float, float
- Left/Right Speed `SX` float
- Jump Speed `SY` float
- Gravity `GY` float
- Bullet Animation `B` std::string (Animation asset to use for bullets)
