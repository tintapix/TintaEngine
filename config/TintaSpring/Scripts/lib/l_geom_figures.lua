require "Scripts/lib/l_util"
--[[ ------------------------------------------------------------------------------
Creates polygon by arounding center with points
	sides - points around
	minLength - minimal radius for random
	maxLength - maximal radius for random
	xCent - x coordinate center
	yCent - y coordinate center
	return polyid
]]------------------------------------------------------------------------------

function s_createPolygon( sides, minRad, maxRad , xCent, yCent, bFill )



	c_assert( sides > 3 and maxRad <= minRad , "s_createPolygon error: polygon was not created!: sides ",sides," maxLength ", minRad," minLength ", maxRad)
	
	
	polygonId = c_creategeom( "geom_polygon" )		
	
	
	c_setgeomprop1b( polygonId, "fill",  bFill )			
	
	angle =  s_genRandTable( 0, 360, sides )
	
	for count = 1, sides do						
		dist  = c_randint( minRad, maxRad )
		xPos, yPos = c_getdist( xCent, yCent, angle[count], dist )		
		c_addgeomprop2i( polygonId, "point",  xPos, yPos )
	end	
	c_calcgeom( polygonId )
	
	return polygonId
end


function s_createRandSpline( points, minRad, maxRad , xCent, yCent, stepSpline )

	c_assert( points > 3 and maxRad >= minRad , "s_createRandSpline error: splinr patch was not created!: sides ",points," maxRad ", maxRad," minRad ", minRad )

	
	splineId = c_creategeom( "geom_spline" )
	c_setgeomprop1n( splineId, "outline_pixel_step", stepSpline )	
	
	--c_setcompprop( splineId, "fill_method",  "fill_solid" )		
	
	--angle =  s_genRandTable( 0, 360, points )
	
	for count = 1, points do				
		angle = c_randint( 0, 360 )
		
		dist  = c_randint( minRad, maxRad )
		xPos, yPos = c_getdist( xCent, yCent, angle, dist )
		
		c_addgeomprop2i( splineId, "point",  xPos, yPos )
	end
	
	c_calcgeom(splineId)
	return splineId
end


function s_createPolygonRandfill( sides, minLength, maxLength , xCent, yCent, fillProb)

	c_assert( points > 3 and maxRad >= minRad , "s_createPolygonRandfill error: splinr patch was not created!: sides ",points," maxRad ", maxRad," minRad ", minRad )
	
	
	c_setgeomprop1b( splineId, "fill",  true )				
	
	c_setgeomprop1n( polygonId, "points_probability",  fillProb )	
	
	angle =  s_genRandTable( 0, 360, sides )
	--util.msg( "s_createPolygon ", " sides ", sides )
	for count = 1, sides do				
		--angle = c_randint( 0, 360 )
		
		dist  = c_randint( minLength, maxLength )
		xPos, yPos = c_getdist( xCent, yCent, angle[count], dist )
		--util.msg("s_createPolygon ", " xPos ", xPos, " yPos ", yPos)
		c_addgeomprop2i( polygonId, "point",  xPos, yPos )
	end
	
	return polygonId
end








--[[ ------------------------------------------------------------------------------
	Creates Petal based on polygon
	radius
	xBegin
	yBegin	
	angle - angle in degrees - petal direction
	return id of the filled geometry object, id of outline geometry object
--]] ------------------------------------------------------------------------------
function s_createPolygonPetal(  radius , xBegin, yBegin, angle )

		
	polyId = c_creategeom( "geom_polygon" )
	polyIdOutLine = c_creategeom( "geom_polygon" )
	
	--c_setcompprop(splineIdOutLine, "outline_pixel_step", step)	
	--if fill ~= nil and fill == true then		
		c_setgeomprop1b( polyId, "fill",  true )			
	--end
	
	
	
	c_addgeomprop2i( polyId, "point",   xBegin, yBegin )
	c_addgeomprop2i( polyIdOutLine, "point",   xBegin, yBegin )
	
	xPos, yPos = c_getdist( xBegin, yBegin, angle - c_randint( 10, 30 ), c_randint( radius/4, radius ) )	
		
	c_addgeomprop2i( polyIdOutLine, "point",  xPos, yPos )
	c_addgeomprop2i( polyId, "point",  xPos, yPos )
	
	
	
	
	
	xPos, yPos = c_getdist( xBegin, yBegin, angle,  radius )	
		
	c_addgeomprop2i( polyIdOutLine, "point",  xPos, yPos )
	c_addgeomprop2i( polyId, "point",  xPos, yPos )
	
	
	
	xPos, yPos = c_getdist( xBegin, yBegin, angle + c_randint( 10, 30 ), c_randint( radius/4, radius ) )	
		
	c_addgeomprop2i ( polyIdOutLine, "point",  xPos, yPos )
	c_addgeomprop2i( polyId, "point",  xPos, yPos )
	
	
	
	c_calcgeom(polyId)
	c_calcgeom(polyIdOutLine)
	return polyId, polyIdOutLine
end


--[[---------------------------------------------------------------------
	Creates polylinewith random points and deviation
	points - points quantity
	xBeg - x begin coordinate
	yBeg - x end coordinate
	xEnd - y begin coordinate
	yEnd - y end coordinate
	minDev - min deviation 
	maxDev - max deviation 
	step  - step parameter for polyline
	returns geometry object id
]]-----------------------------------------------------------------------
function s_createPolyline( points, xBeg, yBeg, xEnd, yEnd, minDev, maxDev, step )


	
	c_assert( points >= 1 and minDev <= maxDev and step >= 0, "s_createPolyline error: wrong parameters points: ",points," or minDev ", minDev," or maxDev ", maxDev, " or step ", step )
	
	
	lineId = c_creategeom( "geom_polyline" )
	c_setgeomprop1n( lineId, "outline_pixel_step", step )	
	
	--c_setcompprop( splineId, "fill_method",  "fill_solid" )		
	
	--angle =  s_genRandTable( 0, 360, points )
	len = s_get_length( xBeg, yBeg, xEnd, yEnd )
	tabPos = s_genRandTable( 0, len, points )
	
	c_addgeomprop2i( lineId, "point",  xBeg, yBeg )
	--c_setpixelb( xBeg, yBeg, 0,0,0,1 )
	local xPre, yPre = c_getdist( xBeg, yBeg, angle, tabPos[1] )
	
	local mainangle = util.c_fromradtodeg( c_getdir( xBeg, yBeg, xEnd, yEnd ) )
	local prelen = 0
	for count = 1, points do	
		
		
		xPre, yPre = c_getdist( xBeg, yBeg, mainangle, prelen+ tabPos[count] )
		--util.msg(xPre," ", yPre)		
		local lenDev = c_randint(minDev, maxDev)
		
		local angle = 90
		if c_randint( 0, 1 ) == 1 then
			angle = 90
		else
			angle = -90
		end		
		local angle_ = mainangle + angle
		--util.msg(angle_)
		--util.msg(lenDev)
		local xp, yp = c_getdist( xPre, yPre, angle_, lenDev )
		--util.msg(xp," ",yp)
		--c_setpixelb(xp, yp, 0,255,0,1)
		
		c_addgeomprop2i( lineId, "point",  xp, yp )
		--xPre, yPre = xp, yp
	end
	
	c_addgeomprop2i( lineId, "point",  xEnd, yEnd )
	--c_setpixelb( xEnd, yEnd, 0,0,0,1 )
	
	c_calcgeom( lineId )
	return lineId
end


--[[---------------------------------------------------------------------
	Creates spline with random nodes and deviation
	points - points quantity
	xBeg - x begin coordinate
	yBeg - x begin coordinate
	xEnd - x begin coordinate
	yEnd - x begin coordinate
	minDev - min deviation 
	maxDev - max deviation 
	step  - step parameter for spline
	returns geometry object id
]]-----------------------------------------------------------------------
function s_createSpline( points, xBeg, yBeg, xEnd, yEnd, minDev, maxDev, step )


	
	c_assert( points >= 1 and minDev <= maxDev and step >= 0, "s_createSpline error: wrong parameters points: ",points," or minDev ", minDev," or maxDev ", maxDev, " or step ", step )
	
	
	lineId = c_creategeom( "geom_spline" )
	c_setgeomprop1n( lineId, "outline_pixel_step", step )	
	
	--c_setcompprop( splineId, "fill_method",  "fill_solid" )		
	
	--angle =  s_genRandTable( 0, 360, points )
	len = s_get_length( xBeg, yBeg, xEnd, yEnd )
	tabPos = s_genRandTable( 0, len, points )
	
	c_addgeomprop2i( lineId, "point",  xBeg, yBeg )
	--c_setpixelb( xBeg, yBeg, 0,0,0,1 )
	local xPre, yPre = c_getdist(xBeg, yBeg, angle, tabPos[1] )
	
	local mainangle = util.c_fromradtodeg( c_getdir( xBeg, yBeg, xEnd, yEnd ) )
	local prelen = 0
	for count = 1, points do	
		
		
		xPre, yPre = c_getdist( xBeg, yBeg, mainangle, prelen + tabPos[count] )
		--util.msg(tabPos[count])		
		local lenDev = c_randint(minDev, maxDev)
		
		local angle = 90
		if c_randint( 0, 1 ) == 1 then
			angle = 90
		else
			angle = -90
		end		
		local angle_ = mainangle + angle
		--util.msg(angle_)
		--util.msg(lenDev)
		local xp, yp = c_getdist( xPre, yPre, angle_, lenDev )
		--util.msg(xp," ",yp)
		--c_setpixelb(xp, yp, 0,255,0,1)
		
		c_addgeomprop2i( lineId, "point",  xp, yp )
		--xPre, yPre = xp, yp
	end
	
	c_addgeomprop2i( lineId, "point",  xEnd, yEnd )
	--c_setpixelb( xEnd, yEnd, 0,0,0,1 )
	
	c_calcgeom( lineId )
	return lineId
end

--[[---------------------------------------------------------------------
	Creates spline with random nodes and deviation
	points - points quantity
	xBeg - x begin coordinate
	yBeg - x begin coordinate
	xEnd - x begin coordinate
	yEnd - x begin coordinate
	minDev - min deviation 
	maxDev - max deviation 
	step  - step parameter for spline
	returns geometry object id
]]-----------------------------------------------------------------------
function s_createSpline2( points, xBeg, yBeg, xEnd, yEnd, minDev, maxDev, step )


	
	c_assert( points >= 1 and minDev <= maxDev and step >= 0, "s_createSpline error: wrong parameters points: ",points," or minDev ", minDev," or maxDev ", maxDev, " or step ", step )
		
	
	lineId = c_creategeom( "geom_spline" )
	c_setgeomprop1n( lineId, "outline_pixel_step", step )	
	
	--c_setcompprop( splineId, "fill_method",  "fill_solid" )		
	
	--angle =  s_genRandTable( 0, 360, points )
	len = s_get_length( xBeg, yBeg, xEnd, yEnd )
	tabPos = s_genRandTable( 0, len, points )
	
	c_addgeomprop2i( lineId, "point",  xBeg, yBeg )
	--c_setpixelb( xBeg, yBeg, 0,0,0,1 )
	local xPre, yPre = c_getdist(xBeg, yBeg, angle, tabPos[1] )
	
	local mainangle = util.c_fromradtodeg( c_getdir( xBeg, yBeg, xEnd, yEnd ) )
	local prelen = 0
	for count = 1, points do	
		
		
		xPre, yPre = c_getdist( xBeg, yBeg, mainangle, prelen + tabPos[count] )
		--util.msg(tabPos[count])		
		local lenDev = c_randint(minDev, maxDev)
		
		local angle = 90
		if c_randint( 0, 1 ) == 1 then
			angle = 90
		else
			angle = -90
		end		
		local angle_ = mainangle + angle
		--util.msg(angle_)
		--util.msg(lenDev)
		local xp, yp = c_getdist( xPre, yPre, angle_, lenDev )
		--util.msg(xp," ",yp)
		--c_setpixelb(xp, yp, 0,255,0,1)
		xBeg = xp
		yBeg =  yp	
		c_addgeomprop2i( lineId, "point",  xp, yp )
		--xPre, yPre = xp, yp
	end
	
	c_addgeomprop2i( lineId, "point",  xEnd, yEnd )
	--c_setpixelb( xEnd, yEnd, 0,0,0,1 )
	
	c_calcgeom( lineId )
	return lineId
end



function s_createCircImg( radius, colorR, colorG, colorB )

	local image = c_createimg( radius*2 + 4, radius*2 + 4 , "basic" )
	c_fillalphaf(0)
	c_fillimgb(colorR, colorG, colorB )
	
	circleId  = c_creategeom( "geom_circle" )	
	c_setgeomprop1n( circleId, "circle_radius",  radius    )
	c_setgeomprop1b( circleId, "fill",  true )	
	c_setgeomprop2n( circleId, "center",  radius, radius )
	c_calcgeom( circleId )		
	
	local pointsQuantity = c_pointscount( circleId )	
	for i = 0, pointsQuantity - 1 do	
		xp,yp  = c_getpoint( circleId, i )	
		c_setalphaf( xp,yp, 1 )
	end
	c_delgeom(circleId)
	
	util.msg( "c_gettasktype ", c_gettasktype() )
	
	if c_gettasktype() == 2 then	
		c_sendimgresult("PNG", image)
		c_delimg(image)
	else	
		return image	
	end
end

