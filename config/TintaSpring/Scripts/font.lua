local font_id1 = c_addfont("fonts/rus.local", 1, 1, 100,255,0 )

local font_id2 = c_copyfont(font_id1)

c_fontcolor(font_id2, 255,0,0 )



local id_image = c_createimg( 3000, 300, "basic" )
c_fillimgb(255,255,255)
c_fillalphab(255)

c_selfont(font_id1)
c_drawtext("Лицемер", 100, 100, 5,1 )
--c_selfont(font_id2)
--c_drawtext("бога нет2", 100, 300, 1,1 )

c_saveimg("png", "c:/foto/font.png")


c_delallimg()
c_delallfont()