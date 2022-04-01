local font_id1 = font.add("fonts/rus.local", 0.1, 0.1, 100,255,0 )

--local font_id2 = c_copyfont(font_id1)

font.setcolor(font_id1, 255,0,0 )



local id_image = image.create( 200, 50, "basic" )
image.fillb(255,255,255)
image.fillchannelb( 3, 255 )

font.select( font_id1 )
font.render("Тест для проверки\nтекст для проверки 2\nтекст для 	проверки 3", 10, 10, 3 ,4 )
--c_selfont(font_id2)

image.save("png", "c:/foto/font.png")


image.eraseall()
font.eraseall()

