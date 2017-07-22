#include "gpu.h"
#include "Main.h"
#include <SFML\System.hpp>
#include <SFML\Graphics\Color.hpp>
#include <vector>

gpu::gpu()
{
}


gpu::~gpu()
{
}


byte gpu::getLCDStatus()
{
	return beakMemory.readMemory(0xFF41);
}

void gpu::setLCDStatus(byte newStatus)
{
	beakMemory.writeMemory(0xFF41, newStatus);
}

void gpu::setLCDMode(byte status)
{
	beakMemory.writeMemory(0xFF41, (byte)((getLCDStatus() & 0xFC) | status));
}

byte gpu::getLCDMode()
{
	return (getLCDStatus() & 0x03);
}

byte gpu::getLCDLYCCheckEnabled()
{
	return (getLCDStatus() & 0x40) >> 6;
}

byte gpu::getLCDLYCompare()
{
	return beakMemory.readMemory(0xFF45);
}

byte gpu::getLCDLY()
{
	return beakMemory.readMemory(0xFF44);
}

byte gpu::getLCDControl()
{
	return beakMemory.readMemory(0xFF40);
}

bool gpu::getLCDOn()
{
	return ((getLCDControl() & 0x80) >> 7) > 0;
}

bool gpu::getWindowTileMapLocation()
{
	return ((getLCDControl() & 0x40) >> 6) > 0;
}

bool gpu::getWindowEnabled()
{
	return ((getLCDControl() & 0x20) >> 5) > 0;
}

bool gpu::getBackGroundWindowTileSetLocation()
{
	return ((getLCDControl() & 0x10) >> 4) > 0;
}

bool gpu::getBackGroundTileMapLocation()
{
	return ((getLCDControl() & 0x08) >> 3) > 0;
}

bool gpu::getSpriteSize()
{
	return ((getLCDControl() & 0x04) >> 2) > 0;
}

bool gpu::getSpriteEnabled()
{
	return ((getLCDControl() & 0x02) >> 1) > 0;
}

bool gpu::getBackGroundEnabled()
{
	return (getLCDControl() & 0x01);
}

//Draw All Tiles: Draws each tile directly from vram in the order they are in Vram. Prints out each tile that is loaded into memory
void gpu::drawAllTiles()
{
	int baseAddress = 0x8000;

	vector<vector<Color>> tile;

	for (int i = 0; i < 360; i++)
	{
		tile.clear();

		int tileOffset = i * 16;

		int tileAddress = baseAddress + tileOffset;

		for (int j = 0; j < 16; j += 2)
		{
			byte rowHalf1 = beakMemory.readMemory(tileAddress + j);
			byte rowHalf2 = beakMemory.readMemory(tileAddress + j + 1);

			vector<Color> row;

			for (int k = 0; k < 8; k++)
			{
				int test = rowHalf1 & 0x01;
				int test2 = rowHalf2 & 0x01;
				int test3 = ((rowHalf1 & 0x01) << 1) | (rowHalf2 & 0x01);
				Color test4 = returnColor(((rowHalf1 & 0x01) << 1) | (rowHalf2 & 0x01));

				row.push_back(returnColor((((rowHalf1 & 0x80) >> 7)) | ((rowHalf2 & 0x80) >> 6)));
				rowHalf1 <<= 1;
				rowHalf2 <<= 1;
			}

			tile.push_back(row);
		}

		drawDebugTile(i, tile);

	}
}
/*
void gpu::drawTiles()
{
	int scrollY = beakMemory.readMemory(0xFF42);
	int scrollX = beakMemory.readMemory(0xFF43);
	int windowY = beakMemory.readMemory(0xFF4A);
	int windowX = beakMemory.readMemory(0xFF4B);
	int mapAddress;
	int baseAddress;

	if (getBackGroundTileMapLocation())
	{
		mapAddress = 0x9C00;
	}
	else
	{
		mapAddress = 0x9800;
	}

	if (getBackGroundWindowTileSetLocation())
	{
		baseAddress = 0x8000;
	}
	else
	{
		baseAddress = 0x8800;
	}



	vector<vector<Color>> tile;

	//for (int i = 0; i < 32 * 32; i++)
	for (int i = 0; i < 32; i++)
	{
		//int finalLocation = (baseAddress == 0x8000) ? baseAddress + (16 * i) : baseAddress + (16 * (i + 0x80));

		int tileY = i / 32;
		int tileX = i - (32 * i);
		int tileIndex = tileY * (32 * tileX);

		int tileIDAddress = mapAddress + tileIndex;

		int tileID = beakMemory.readMemory(tileIDAddress);
		int tileOffset = tileID * 16;

		int tileAddress = baseAddress + tileOffset;

		for (int j = 0; j < 16; j += 2)
		{
			//byte rowHalf1 = beakMemory.readMemory(finalLocation + j);
			//byte rowHalf2 = beakMemory.readMemory(finalLocation + j + 1);
			byte rowHalf1 = beakMemory.readMemory(tileAddress + j);
			byte rowHalf2 = beakMemory.readMemory(tileAddress + j + 1);

			vector<Color> row;

			for (int k = 0; k < 8; k++)
			{
				row.push_back(returnColor((rowHalf1 & 0x01) + (rowHalf2 & 0x01)));
				rowHalf1 >>= 1;
				rowHalf2 >>= 1;
			}

			tile.push_back(row);
		}

		drawTile(i, tile);
	}
}
*/


//DrawLineFromMap: Draws a single specific line of the background map
void gpu::drawLineFromBGMap(int lineY)
{

	int mapAddress;
	int baseAddress;

	if (getBackGroundTileMapLocation())
	{
		mapAddress = 0x9C00;
	}
	else
	{
		mapAddress = 0x9800;
	}

	if (getBackGroundWindowTileSetLocation())
	{
		baseAddress = 0x8000;
	}
	else
	{
		baseAddress = 0x8800;
	}

	int lineToDraw = lineY % 8;

	int tileY = 0;
	int tileX = 0;
	int tileIndex = 0;
	int tileIDAddress = 0;
	int tileID = 0;
	int tileOffset = 0;
	int tileAddress = 0;
	byte rowHalf1 = 0;
	byte rowHalf2 = 0;

	for (int i = 0; i < 32; i++)
	{
		tileY = (lineY) / 8;
		tileX = (i);
		tileIndex = tileX + (32 * tileY);
		tileIDAddress = mapAddress + tileIndex;

		tileID = beakMemory.readMemory(tileIDAddress);

		if (baseAddress == 0x8800)
		{
			if (tileID > 0x7F)
			{
				tileID -= 0x80;
			}
			else
			{
				tileID += 0x80;
			}
		}

		tileOffset = tileID * 16;
		tileAddress = baseAddress + tileOffset;


		rowHalf1 = beakMemory.readMemory(tileAddress + (lineToDraw * 2));
		rowHalf2 = beakMemory.readMemory(tileAddress + (lineToDraw * 2) + 1);

		for (int j = 0; j < 8; j++)
		{
			beakWindow.setBGPixel((i * 8) + j, lineY, returnColor(((rowHalf1 & 0x80) >> 7) | ((rowHalf2 & 0x80) >> 6), 0));
			rowHalf1 <<= 1;
			rowHalf2 <<= 1;

		}
		
	}

}

void gpu::drawLineFromWindowMap(int lineY)
{

	int mapAddress;
	int baseAddress;

	if (getWindowTileMapLocation())
	{
		mapAddress = 0x9C00;
	}
	else
	{
		mapAddress = 0x9800;
	}

	if (getBackGroundWindowTileSetLocation())
	{
		baseAddress = 0x8000;
	}
	else
	{
		baseAddress = 0x8800;
	}

	int lineToDraw = lineY % 8;

	int tileY = 0;
	int tileX = 0;
	int tileIndex = 0;
	int tileIDAddress = 0;
	int tileID = 0;
	int tileOffset = 0;
	int tileAddress = 0;
	byte rowHalf1 = 0;
	byte rowHalf2 = 0;

	for (int i = 0; i < 32; i++)
	{

		tileY = (lineY) / 8;
		tileX = (i);
		tileIndex = tileX + (32 * tileY);
		tileIDAddress = mapAddress + tileIndex;

		tileID = beakMemory.readMemory(tileIDAddress);

		if (baseAddress == 0x8800)
		{
			if (tileID > 0x7F)
			{
				tileID -= 0x80;
			}
			else
			{
				tileID += 0x80;
			}
		}

		tileOffset = tileID * 16;
		tileAddress = baseAddress + tileOffset;

		rowHalf1 = beakMemory.readMemory(tileAddress + (lineToDraw * 2));
		rowHalf2 = beakMemory.readMemory(tileAddress + (lineToDraw * 2) + 1);

		for (int j = 0; j < 8; j++)
		{
			beakWindow.setWindowPixel((i * 8) + j, lineY, returnColor(((rowHalf1 & 0x80) >> 7) | ((rowHalf2 & 0x80) >> 6), 0));
			rowHalf1 &= 0x7F;
			rowHalf1 <<= 1;
			rowHalf2 &= 0x7F;
			rowHalf2 <<= 1;

		}

	}


}

#include "Binary.h"

void gpu::drawLineFromSpriteMap(int lineY)
{
	//Todo: Support priority.

	int mapAddress = 0xFE00;
	int baseAddress = 0x8000;

	//False: 8x8 | True: 8x16
	bool spriteSize = getSpriteSize();

	byte y = 0;
	byte x = 0;
	int tileY = 0;
	int tileX = 0;
	int tileIndex = 0;
	int tileIDAddress = 0;
	int tileID = 0;
	int tileOffset = 0;
	int tileAddress = 0;
	byte rowHalf1 = 0;
	byte rowHalf2 = 0;
	int lineToDraw = 0;

	byte tileNumber = 0;
	byte tileFlags = 0;
	bool priority = false;
	bool yFlip = false;
	bool xFlip = false;
	bool palette = false;

	for (int i = 0; i < 40; i++)
	{
		y = beakMemory.readMemory(mapAddress + (i * 4));

		if (y > 8)
		{
			y -= 16;
			x = beakMemory.readMemory(mapAddress + (i * 4) + 1) - 8;

			bool isSpriteOnLine = (y <= lineY) && ((y + ((spriteSize == 0) ? 8 : 16)) > lineY);
			//(y <= lineY)  if the start of the sprite is at lineY or before it
			//(y + (spriteSize == 0) ? 8 : 16)) > lineY) if the end of the sprite is past lineY (By way of commenting, this isn't the problem)


			if (isSpriteOnLine)
			{
				lineToDraw = lineY - y;

				tileNumber = beakMemory.readMemory(mapAddress + (i * 4) + 2);
				tileFlags = beakMemory.readMemory(mapAddress + (i * 4) + 3);
				priority = ((tileFlags & 0x80) >> 7) > 0; //If 1, displays in front of window. Otherwise is below window and above BG
				yFlip = ((tileFlags & 0x40) >> 6) > 0; //Vertically flipped if 1, else 0.
				xFlip = ((tileFlags & 0x20) >> 5) > 0; //Horizontally flipped if 1, else 0;
				palette = ((tileFlags & 0x10) >> 4) > 0; //Palette is OBJ0PAL if 0, else OBJ1PAL

				tileOffset = tileNumber * 16;
				tileAddress = baseAddress + tileOffset;

				if (yFlip)
				{
					lineToDraw = 8 - lineToDraw;
				}

				rowHalf1 = beakMemory.readMemory(tileAddress + (lineToDraw * 2));
				rowHalf2 = beakMemory.readMemory(tileAddress + (lineToDraw * 2) + 1);

				if (xFlip)
				{
					rowHalf1 = reverseBits(rowHalf1);
					rowHalf2 = reverseBits(rowHalf2);
				}

				for (int j = 0; j < 8; j++)
				{
					byte colorNumber = ((rowHalf1 & 0x80) >> 7) | ((rowHalf2 & 0x80) >> 6);
					Color color;

					if (colorNumber > 0)
					{
						color = returnColor(colorNumber, palette + 1);
						beakWindow.setSpritePixel(x + j, lineY, color);
					}

					rowHalf1 <<= 1;
					rowHalf2 <<= 1;
				}

			}
		}
	}
}

//DrawTile: Draws tile data as given to location of tile number given
void gpu::drawDebugTile(int tileNumber, vector<vector<Color>> tile)
{
	//int y = tileNumber / 32;
	//int x = tileNumber - (32 * y);

	int y = tileNumber / 20;
	int x = tileNumber - (20 * y);

	cout << "helloKitty" << endl;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			beakWindow.setDebugPixel((x * 8) + j, (y * 8) + i, tile[0][j]);
		}

		tile.erase(tile.begin());
	}

}

byte gpu::getScrollX()
{
	return beakMemory.readMemory(0xFF43);
}

byte gpu::getScrollY()
{
	return beakMemory.readMemory(0xFF42);
}

byte gpu::getWindowX()
{
	return beakMemory.readMemory(0xFF4B);
}

byte gpu::getWindowY()
{
	return beakMemory.readMemory(0xFF4A);
}

Color gpu::returnColor(int colorNumber)
{
	return gameBeakPalette[colorNumber + (paletteSetting << 2)];
}

Color gpu::returnColor(int colorNumber, int palette)
{
	//Palette: 0 = BGP
	//Palette: 1 = 0BP0
	//Palette: 2 = 0BP1

	//Palette decides between BGP (0xFF47), 0BP0 (0xFF48), and 0BP1 (0xFF49)
	//Color number decides which color slot is selected from the palette data.
	//The value returned from the palette data is then used to index from the emulator's palette array.

	//byte paletteData = beakMemory.readMemory(0xFF47 + palette);
	//colorNumber = (paletteData & (3 << (colorNumber * 2))) >> (colorNumber * 2);//(colorNumber + 1);
	//return gameBeakPalette[colorNumber + paletteSetting << 2)];

	return gameBeakPalette[((beakMemory.readMemory(0xFF47 + palette) & (3 << (colorNumber * 2))) >> (colorNumber * 2)) + (paletteSetting << 2)];


	//The palette variable is being used to select the memory location of the palette
	//The palette data that is retrieved is made up of 8 bits. 4 colors represented by 2 bits each.
	//The 2 bit groups can represent numbers 0-3 and represent how light or dark the color we want is supposed to be.
	//Once we retrieve the correct grouping it will be used to index the exact color to be used from the emulator's palette array.
	//To retrieve the appropriate grouping we have to mask the 2 bits we want. The number 3 will
	//retrieve the first 2 bits, and we can shift the number three to select the next three groupings if needed.
	//We use the color number to decide how much 3 needs to be shifted (if at all) to get the set of bits we want.
	//We then use it again to decide how much they need to be shifted right (if at all) to reduce the number to 0-3
	//Finally, we have the index we will use to index the palette, and we can add the value of paletteSettng * 4 (or << 2) (multiples of 4 to select correct group)
	//To index the grey or pink set of colors depending on the mode.


	//Below is identical functionality, but simply much slower.
	//return gameBeakPalette[ returnHalfNibble( returnPalette(palette), colorNumber) ];
}

byte gpu::returnPalette(byte palette)
{
	//Palette: 0 = BGP
	//Palette: 1 = 0BP0
	//Palette: 2 = 0BP1
	return beakMemory.readMemory(0xFF47 + palette);
}

void gpu::loadPalettesFromXML(ifstream file)
{
	string line;

	list<int> colorValues;

	while (getline(file, line))
	{
		bool test1 = (line.find("<r>") != string::npos) && (line.find("</r>") != string::npos);
		bool test2 = (line.find("<g>") != string::npos) && (line.find("</g>") != string::npos);
		bool test3 = (line.find("<b>") != string::npos) && (line.find("</b>") != string::npos);

		if (test1 || test2 || test3)
		{
			int first = line.find_first_of('>') + 1;
			int last = line.find_last_of('<');

			line = line.substr(first, last - first);

			colorValues.push_back(stoi(line));
		}
	}

	if ((colorValues.size() / 12) > 0)
	{
		int paletteOffset = 0;
		int colorOffset = 0;

		while (colorValues.size() >= (3 * 4))
		{
			for (int i = 0; i < 4; i++)
			{
				int r = colorValues.front();
				colorValues.pop_front();
				int g = colorValues.front();
				colorValues.pop_front();
				int b = colorValues.front();
				colorValues.pop_front();

				Color color = Color(r, g, b, 255);


				beakGPU.gameBeakPalette[(paletteOffset * 4) + colorOffset] = color;

				if (colorOffset >= 3)
				{
					colorOffset = 0;
				}
				else
				{
					colorOffset++;
				}
			}

			paletteOffset++;
		}
	}

	file.close();
}

ifstream gpu::openCreatePalettesXML()
{
	//open XML palette file
	char path1[MAX_PATH];
	string path;
	GetModuleFileNameA(NULL, path1, MAX_PATH);
	path = string(path1);
	path = path.substr(0, path.find_last_of('\\') + 1);

	ifstream colorFile(path + "palettes.xml");
	if (colorFile.fail())
	{
		ofstream file(path + "palettes.xml");
		if (file.is_open())
		{
			file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
			file << "<colorschemes>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>BlackWhite</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>255</g>" << endl;
			file << "\t\t\t<b>255</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>185</r>" << endl;
			file << "\t\t\t<g>185</g>" << endl;
			file << "\t\t\t<b>185</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>105</r>" << endl;
			file << "\t\t\t<g>105</g>" << endl;
			file << "\t\t\t<b>105</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>0</r>" << endl;
			file << "\t\t\t<g>0</g>" << endl;
			file << "\t\t\t<b>0</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>Green</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>224</r>" << endl;
			file << "\t\t\t<g>255</g>" << endl;
			file << "\t\t\t<b>235</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>136</r>" << endl;
			file << "\t\t\t<g>208</g>" << endl;
			file << "\t\t\t<b>88</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>52</r>" << endl;
			file << "\t\t\t<g>183</g>" << endl;
			file << "\t\t\t<b>60</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>8</r>" << endl;
			file << "\t\t\t<g>71</g>" << endl;
			file << "\t\t\t<b>3</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>Pink</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>241</g>" << endl;
			file << "\t\t\t<b>254</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>214</g>" << endl;
			file << "\t\t\t<b>245</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>131</g>" << endl;
			file << "\t\t\t<b>217</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>72</r>" << endl;
			file << "\t\t\t<g>24</g>" << endl;
			file << "\t\t\t<b>59</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>PinkAlt</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>240</g>" << endl;
			file << "\t\t\t<b>245</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>186</g>" << endl;
			file << "\t\t\t<b>222</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>116</g>" << endl;
			file << "\t\t\t<b>217</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>82</r>" << endl;
			file << "\t\t\t<g>5</g>" << endl;
			file << "\t\t\t<b>40</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>BubbleGum</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>240</g>" << endl;
			file << "\t\t\t<b>254</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>186</g>" << endl;
			file << "\t\t\t<b>222</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>96</g>" << endl;
			file << "\t\t\t<b>194</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>111</r>" << endl;
			file << "\t\t\t<g>6</g>" << endl;
			file << "\t\t\t<b>57</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>GrapeCherry</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>227</g>" << endl;
			file << "\t\t\t<b>254</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>203</r>" << endl;
			file << "\t\t\t<g>160</g>" << endl;
			file << "\t\t\t<b>186</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>151</r>" << endl;
			file << "\t\t\t<g>80</g>" << endl;
			file << "\t\t\t<b>118</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>90</r>" << endl;
			file << "\t\t\t<g>0</g>" << endl;
			file << "\t\t\t<b>51</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>KiGB</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>156</r>" << endl;
			file << "\t\t\t<g>185</g>" << endl;
			file << "\t\t\t<b>22</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>140</r>" << endl;
			file << "\t\t\t<g>170</g>" << endl;
			file << "\t\t\t<b>20</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>48</r>" << endl;
			file << "\t\t\t<g>100</g>" << endl;
			file << "\t\t\t<b>48</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>16</r>" << endl;
			file << "\t\t\t<g>63</g>" << endl;
			file << "\t\t\t<b>16</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>BGB</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>224</r>" << endl;
			file << "\t\t\t<g>248</g>" << endl;
			file << "\t\t\t<b>208</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>136</r>" << endl;
			file << "\t\t\t<g>192</g>" << endl;
			file << "\t\t\t<b>112</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>52</r>" << endl;
			file << "\t\t\t<g>104</g>" << endl;
			file << "\t\t\t<b>86</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>8</r>" << endl;
			file << "\t\t\t<g>24</g>" << endl;
			file << "\t\t\t<b>32</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>NO$GMB</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>252</r>" << endl;
			file << "\t\t\t<g>232</g>" << endl;
			file << "\t\t\t<b>140</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>220</r>" << endl;
			file << "\t\t\t<g>180</g>" << endl;
			file << "\t\t\t<b>92</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>152</r>" << endl;
			file << "\t\t\t<g>124</g>" << endl;
			file << "\t\t\t<b>60</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>76</r>" << endl;
			file << "\t\t\t<g>60</g>" << endl;
			file << "\t\t\t<b>28</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>PLAYGUY</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>238</g>" << endl;
			file << "\t\t\t<b>172</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>172</r>" << endl;
			file << "\t\t\t<g>164</g>" << endl;
			file << "\t\t\t<b>115</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>90</r>" << endl;
			file << "\t\t\t<g>82</g>" << endl;
			file << "\t\t\t<b>57</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>0</r>" << endl;
			file << "\t\t\t<g>0</g>" << endl;
			file << "\t\t\t<b>0</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;

			file << "\t\t<name>DREAMGBC</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>255</g>" << endl;
			file << "\t\t\t<b>255</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>0</r>" << endl;
			file << "\t\t\t<g>178</g>" << endl;
			file << "\t\t\t<b>181</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>0</r>" << endl;
			file << "\t\t\t<g>134</g>" << endl;
			file << "\t\t\t<b>140</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>0</r>" << endl;
			file << "\t\t\t<g>0</g>" << endl;
			file << "\t\t\t<b>0</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t<scheme>" << endl;


			file << "\t\t<name>HEBOWIN</name>\n" << endl;
			file << "\t\t<color1>" << endl;
			file << "\t\t\t<r>255</r>" << endl;
			file << "\t\t\t<g>255</g>" << endl;
			file << "\t\t\t<b>255</b>" << endl;
			file << "\t\t</color1>\n" << endl;

			file << "\t\t<color2>" << endl;
			file << "\t\t\t<r>127</r>" << endl;
			file << "\t\t\t<g>204</g>" << endl;
			file << "\t\t\t<b>127</b>" << endl;
			file << "\t\t</color2>\n" << endl;

			file << "\t\t<color3>" << endl;
			file << "\t\t\t<r>51</r>" << endl;
			file << "\t\t\t<g>153</g>" << endl;
			file << "\t\t\t<b>178</b>" << endl;
			file << "\t\t</color3>\n" << endl;

			file << "\t\t<color4>" << endl;
			file << "\t\t\t<r>0</r>" << endl;
			file << "\t\t\t<g>0</g>" << endl;
			file << "\t\t\t<b>0</b>" << endl;
			file << "\t\t</color4>\n" << endl;

			file << "\t</scheme>\n" << endl;
			file << "\t</colorschemes>\n" << endl;
		}

		file.close();
		colorFile.open(path + "palettes.xml");


	}

	return colorFile;
}
