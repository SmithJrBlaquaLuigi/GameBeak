#include "PostProcessing.h"


Color mixColors(Color color1, Color color2)
{
	return Color((color1.r + color2.r) / 2, (color1.g + color2.g) / 2, (color1.b + color2.b) / 2, 0xFF);
}

Color mixColors(Color color1, Color color2, float div)
{
	return Color((color1.r + color2.r) / div, (color1.g + color2.g) / div, (color1.b + color2.b) / div, 0xFF);
}

Color linearInterpolation(Color color1, Color color2, float div)
{
	return Color((color1.r + (color2.r - color1.r) * div), (color1.g + (color2.g - color1.g) * div), (color1.b + (color2.b - color1.b) * div), 0xFF);
}

Color cosineInterpolation(Color color1, Color color2, float div)
{
	return Color((color1.r * (1.0 - div) + color2.r * div), (color1.g * (1.0 - div) + color2.g * div), (color1.b * (1.0 - div) + color2.b * div), 0xFF);
}

Color cosineInterpolation4(Color color1, Color color2, Color color3, Color color4, float div)
{
	Color topInterpolation = cosineInterpolation(color1, color2, div);
	Color bottomInterpolation = cosineInterpolation(color3, color4, div);

	return cosineInterpolation(topInterpolation, bottomInterpolation, div);
}

Image eagleFilter(Image screen)
{
	Image scaledScreen;
	scaledScreen.create(screen.getSize().x * 2, screen.getSize().y * 2, Color(0,0,0,255));

	Color pixel1;
	Color pixel2;
	Color pixel3;
	Color pixel4;

	for (int x = 1; x < (screen.getSize().x - 1); x++) //This ensures every pixel indexed will not be an edge. It will always have 8 surrounding pixels.
	{
		for (int y = 1; y < (screen.getSize().y - 1); y++)
		{

			Color currentPixel = screen.getPixel(x, y);
			Color compareColor = screen.getPixel(x - 1, y); //Left

			if ((screen.getPixel(x - 1, y - 1) == compareColor) && (screen.getPixel(x, y - 1) == compareColor)) //Up-Left & Up
			{
				pixel1 = compareColor;
			}
			else
			{
				pixel1 = currentPixel;
			}

			compareColor = screen.getPixel(x, y - 1); //Up
			if ((screen.getPixel(x + 1, y - 1) == compareColor) && (screen.getPixel(x + 1, y) == compareColor)) //Up-Right & Right
			{
				pixel2 = compareColor;
			}
			else
			{
				pixel2 = currentPixel;
			}

			compareColor = screen.getPixel(x - 1, y); //Left
			if ((screen.getPixel(x - 1, y - 1) == compareColor) && (screen.getPixel(x, y - 1) == compareColor)) //Down-Left & Down
			{
				pixel3 = compareColor;
			}
			else
			{
				pixel3 = currentPixel;
			}

			compareColor = screen.getPixel(x + 1, y); //Right
			if ((screen.getPixel(x + 1, y + 1) == compareColor) && (screen.getPixel(x, y + 1) == compareColor)) //Down-Right & Down
			{
				pixel4 = compareColor;
			}
			else
			{
				pixel4 = currentPixel;
			}

			scaledScreen.setPixel(x * 2, y * 2, pixel1);
			scaledScreen.setPixel((x * 2) + 1, y * 2, pixel2);
			scaledScreen.setPixel(x * 2, (y * 2) + 1, pixel3);
			scaledScreen.setPixel((x * 2) + 1, (y * 2) + 1, pixel4);

		}
	}
	/*
	for (int i = 0; i < 160; i++)
	{
		Color oldPixel1 = screen.getPixel(i, 0);

		scaledScreen.setPixel((i * 2) + 1, 0, oldPixel1);
		scaledScreen.setPixel((i * 2), 0, oldPixel1);
		scaledScreen.setPixel((i * 2) + 1, 1, oldPixel1);
		scaledScreen.setPixel((i * 2), 1, oldPixel1);

		Color oldPixel2 = screen.getPixel(i, 143);

		scaledScreen.setPixel((i * 2) + 1, (143 * 2), oldPixel2);
		scaledScreen.setPixel((i * 2), (143 * 2) + 1, oldPixel2);
		scaledScreen.setPixel((i * 2) + 1, (143 * 2) + 1, oldPixel2);
		scaledScreen.setPixel((i * 2), (143 * 2), oldPixel2);
	}

	for (int i = 1; i < 143; i++)
	{
		Color oldPixel1 = screen.getPixel(0, i);

		scaledScreen.setPixel(0, (i * 2) + 1, oldPixel1);
		scaledScreen.setPixel(0, (i * 2), oldPixel1);
		scaledScreen.setPixel(1, (i * 2) + 1, oldPixel1);
		scaledScreen.setPixel(1, (i * 2), oldPixel1);

		Color oldPixel2 = screen.getPixel(159, i);

		scaledScreen.setPixel((159 * 2), (i * 2) + 1, oldPixel2);
		scaledScreen.setPixel((159 * 2) + 1, (i * 2), oldPixel2);
		scaledScreen.setPixel((159 * 2) + 1, (i * 2) + 1, oldPixel2);
		scaledScreen.setPixel((159 * 2), (i * 2), oldPixel2);
	}
	
	*/

	return scaledScreen;
}

Image scale2XFilter(Image screen)
{
	Image scaledScreen;
	scaledScreen.create(screen.getSize().x * 2, screen.getSize().y * 2, Color(0, 0, 0, 255));

	Color pixel1;
	Color pixel2;
	Color pixel3;
	Color pixel4;

	for (int x = 1; x < (screen.getSize().x - 1); x++) //This ensures every pixel indexed will not be an edge. It will always have 8 surrounding pixels.
	{
		for (int y = 1; y < (screen.getSize().y - 1); y++)
		{

			Color currentPixel = screen.getPixel(x, y);
			Color upColor = screen.getPixel(x, y - 1); //Left
			Color downColor = screen.getPixel(x, y + 1); //Left
			Color leftColor = screen.getPixel(x - 1, y); //Left
			Color rightColor = screen.getPixel(x + 1, y); //Left

			if ( (leftColor == upColor) && (upColor != rightColor) && (leftColor != downColor) )
			{
				pixel1 = leftColor;
			}
			else
			{
				pixel1 = currentPixel;
			}

			if ( (upColor == rightColor) && (upColor != leftColor) && (rightColor != downColor) )
			{
				pixel2 = rightColor;
			}
			else
			{
				pixel2 = currentPixel;
			}

			if ( (leftColor == downColor) && (leftColor != upColor) && (downColor != rightColor))
			{
				pixel3 = leftColor;
			}
			else
			{
				pixel3 = currentPixel;
			}

			if ( (downColor == rightColor) && (leftColor != downColor) && (upColor != rightColor))
			{
				pixel4 = rightColor;
			}
			else
			{
				pixel4 = currentPixel;
			}

			scaledScreen.setPixel(x * 2, y * 2, pixel1);
			scaledScreen.setPixel((x * 2) + 1, y * 2, pixel2);
			scaledScreen.setPixel(x * 2, (y * 2) + 1, pixel3);
			scaledScreen.setPixel((x * 2) + 1, (y * 2) + 1, pixel4);

		}
	}
	/*
	for (int i = 0; i < 160; i++)
	{
	Color oldPixel1 = screen.getPixel(i, 0);

	scaledScreen.setPixel((i * 2) + 1, 0, oldPixel1);
	scaledScreen.setPixel((i * 2), 0, oldPixel1);
	scaledScreen.setPixel((i * 2) + 1, 1, oldPixel1);
	scaledScreen.setPixel((i * 2), 1, oldPixel1);

	Color oldPixel2 = screen.getPixel(i, 143);

	scaledScreen.setPixel((i * 2) + 1, (143 * 2), oldPixel2);
	scaledScreen.setPixel((i * 2), (143 * 2) + 1, oldPixel2);
	scaledScreen.setPixel((i * 2) + 1, (143 * 2) + 1, oldPixel2);
	scaledScreen.setPixel((i * 2), (143 * 2), oldPixel2);
	}

	for (int i = 1; i < 143; i++)
	{
	Color oldPixel1 = screen.getPixel(0, i);

	scaledScreen.setPixel(0, (i * 2) + 1, oldPixel1);
	scaledScreen.setPixel(0, (i * 2), oldPixel1);
	scaledScreen.setPixel(1, (i * 2) + 1, oldPixel1);
	scaledScreen.setPixel(1, (i * 2), oldPixel1);

	Color oldPixel2 = screen.getPixel(159, i);

	scaledScreen.setPixel((159 * 2), (i * 2) + 1, oldPixel2);
	scaledScreen.setPixel((159 * 2) + 1, (i * 2), oldPixel2);
	scaledScreen.setPixel((159 * 2) + 1, (i * 2) + 1, oldPixel2);
	scaledScreen.setPixel((159 * 2), (i * 2), oldPixel2);
	}

	*/

	return scaledScreen;
}

Image scale3XFilter(Image screen)
{
	Image scaledScreen;
	scaledScreen.create(screen.getSize().x * 3, screen.getSize().y * 3, Color(0, 0, 0, 255));

	Color pixel0;
	Color pixel1;
	Color pixel2;

	Color pixel3;
	Color pixel4;
	Color pixel5;

	Color pixel6;
	Color pixel7;
	Color pixel8;

	for (int x = 1; x < (screen.getSize().x - 1); x++) //This ensures every pixel indexed will not be an edge. It will always have 8 surrounding pixels.
	{
		for (int y = 1; y < (screen.getSize().y - 1); y++)
		{

			Color currentPixel = screen.getPixel(x, y);

			Color upColor = screen.getPixel(x, y - 1); 
			Color downColor = screen.getPixel(x, y + 1); 
			Color leftColor = screen.getPixel(x - 1, y); 
			Color rightColor = screen.getPixel(x + 1, y);

			Color upLeftColor = screen.getPixel(x - 1, y - 1);
			Color upRightColor = screen.getPixel(x + 1, y - 1);
			Color downLeftColor = screen.getPixel(x - 1, y + 1);
			Color downRightColor = screen.getPixel(x + 1, y + 1);

			//E0
			if ((leftColor ==  upColor) && (upColor != rightColor) && (leftColor != downColor))
			{
				pixel0 = leftColor;
			}
			else
			{
				pixel0 = currentPixel;
			}

			//E1
			if ( ( (leftColor == upColor) && (upColor != rightColor) && (leftColor != downColor) && (currentPixel != upRightColor) )
				|| ( (upColor == downColor) && (leftColor != upColor) && (downColor != rightColor) && (currentPixel != upLeftColor) ))
			{
				pixel1 = leftColor;
			}
			else
			{
				pixel1 = currentPixel;
			}

			//E2
			if ( (upColor == rightColor) && (upColor != leftColor) && (rightColor != downColor) )
			{
				pixel2 = rightColor;
			}
			else
			{
				pixel2 = currentPixel;
			}

			//E3
			if ( ((leftColor == upColor) &&  (upColor != rightColor) && (leftColor !=  downColor) && (currentPixel != downLeftColor))
				|| ( (leftColor == downColor) && (leftColor != upColor) && (downColor != rightColor) && (currentPixel != upLeftColor)))
			{
				pixel3 = leftColor;
			}
			else
			{
				pixel3 = currentPixel;
			}

			//E4
			pixel4 = currentPixel;


			//E5
			if ( ( (upColor == rightColor) && (upColor != leftColor) && (rightColor != downColor) && (currentPixel != downLeftColor) )  || ( (downColor == rightColor) && (leftColor != downColor) && (upColor != rightColor) && (currentPixel != upRightColor) ) )
			{
				pixel5 = rightColor;
			}
			else
			{
				pixel5 = currentPixel;
			}

			//E6
			if ( (leftColor == downColor) && (leftColor != upColor) && (downColor != rightColor) )
			{
				pixel6 = leftColor;
			}
			else
			{
				pixel6 = currentPixel;
			}

			//E7
			if ( ( (leftColor == downColor) && (leftColor != upColor) && (downColor != rightColor) && (currentPixel != downRightColor) ) 
				|| ( (downColor == rightColor) && (leftColor != downColor) && (upColor != rightColor) && (currentPixel != downLeftColor)))
			{
				pixel7 = downColor;
			}
			else
			{
				pixel7 = currentPixel;
			}

			//E8
			if ( (downColor == rightColor) && (leftColor != downColor) && (upColor != rightColor))
			{
				pixel8 = rightColor;
			}
			else
			{
				pixel8 = currentPixel;
			}


			scaledScreen.setPixel(x * 3, y * 3, pixel0);
			scaledScreen.setPixel((x * 3) + 1, y * 3, pixel1);
			scaledScreen.setPixel((x * 3) + 2, y * 3, pixel2);
			scaledScreen.setPixel(x * 3, (y * 3) + 1, pixel3);
			scaledScreen.setPixel((x * 3) + 1, (y * 3) + 1, pixel4);
			scaledScreen.setPixel((x * 3) + 2, (y * 3) + 1, pixel5);
			scaledScreen.setPixel((x * 3), (y * 3) + 2, pixel6);
			scaledScreen.setPixel((x * 3) + 1, (y * 3) + 2, pixel7);
			scaledScreen.setPixel((x * 3) + 2, (y * 3) + 2, pixel8);

		}
	}
	/*
	for (int i = 0; i < 160; i++)
	{
	Color oldPixel1 = screen.getPixel(i, 0);

	scaledScreen.setPixel((i * 2) + 1, 0, oldPixel1);
	scaledScreen.setPixel((i * 2), 0, oldPixel1);
	scaledScreen.setPixel((i * 2) + 1, 1, oldPixel1);
	scaledScreen.setPixel((i * 2), 1, oldPixel1);

	Color oldPixel2 = screen.getPixel(i, 143);

	scaledScreen.setPixel((i * 2) + 1, (143 * 2), oldPixel2);
	scaledScreen.setPixel((i * 2), (143 * 2) + 1, oldPixel2);
	scaledScreen.setPixel((i * 2) + 1, (143 * 2) + 1, oldPixel2);
	scaledScreen.setPixel((i * 2), (143 * 2), oldPixel2);
	}

	for (int i = 1; i < 143; i++)
	{
	Color oldPixel1 = screen.getPixel(0, i);

	scaledScreen.setPixel(0, (i * 2) + 1, oldPixel1);
	scaledScreen.setPixel(0, (i * 2), oldPixel1);
	scaledScreen.setPixel(1, (i * 2) + 1, oldPixel1);
	scaledScreen.setPixel(1, (i * 2), oldPixel1);

	Color oldPixel2 = screen.getPixel(159, i);

	scaledScreen.setPixel((159 * 2), (i * 2) + 1, oldPixel2);
	scaledScreen.setPixel((159 * 2) + 1, (i * 2), oldPixel2);
	scaledScreen.setPixel((159 * 2) + 1, (i * 2) + 1, oldPixel2);
	scaledScreen.setPixel((159 * 2), (i * 2), oldPixel2);
	}

	*/

	return scaledScreen;
}

Image emboldenFilter(Image screen)
{
	for (int x = 0; x < 159; x++)
	{
		for (int y = 0; y < 144; y++)
		{
			sf::Color pixel1 = screen.getPixel(x, y);
			sf::Color pixel2 = screen.getPixel(x + 1, y);

			screen.setPixel(x, y, pixel1*pixel2);
		}
	}

	return screen;
}

Image customFilter(Image screen)
{
	for (int x = 0; x < 160; x++)
	{
		for (int y = 0; y < 144; y++)
		{
			Color blackPixel = beakGPU.returnColor(3);
			Color pixel1 = screen.getPixel(x, y);

			if (pixel1 == blackPixel)
			{
				if (y < 143)
				{
					if (x > 0)
					{
						if (screen.getPixel(x - 1, y + 1) == blackPixel) //if Down-Left pixel is black
						{
							screen.setPixel(x - 1, y, mixColors(pixel1, screen.getPixel(x - 1, y), 3.5)); //Left
						}
					}

					if (x < 159)
					{
						if (screen.getPixel(x + 1, y + 1) == blackPixel) //if Down-Right pixel is black
						{
							screen.setPixel(x + 1, y, mixColors(pixel1, screen.getPixel(x + 1, y), 3.5)); //Right
						}
					}

					//screen.setPixel(x, y + 1, mixColors(pixel1, screen.getPixel(x, y + 1))); //Down
				}
			}
		}
	}

	return screen;
}

Image blurFilter(Image screen)
{
	
	for (int x = 0; x < 160; x++)
	{
		for (int y = 0; y < 144; y++)
		{
			Color pixel1 = screen.getPixel(x, y);

			if (y > 0)
			{
				if (x > 0)
				{
					pixel1 = mixColors(pixel1, screen.getPixel(x - 1, y - 1)); //Up-Left
					pixel1 = mixColors(pixel1, screen.getPixel(x - 1, y)); //Left
				}

				if (x < 159)
				{
					pixel1 = mixColors(pixel1, screen.getPixel(x + 1, y - 1)); //Up-Right
					pixel1 = mixColors(pixel1, screen.getPixel(x + 1, y)); //Right
				}

				pixel1 = mixColors(pixel1, screen.getPixel(x, y - 1)); //Up

			}

			if (y < 143)
			{
				if (x > 0)
				{
					pixel1 = mixColors(pixel1, screen.getPixel(x - 1, y + 1)); //Down-Left
				}

				if (x < 160)
				{
					pixel1 = mixColors(pixel1, screen.getPixel(x + 1, y + 1)); //Down-Right
				}

				pixel1 = mixColors(pixel1, screen.getPixel(x, y + 1)); //Down
			}

			screen.setPixel(x, y, pixel1);
		}
	}
	return screen;
}

Image zigZagFilter(Image screen)
{
	for (int x = 0; x < 160; x++)
	{
		for (int y = 0; y < 144; y += 2)
		{
			sf::Color blackPixel = beakGPU.returnColor(3);
			sf::Color pixel1 = screen.getPixel(x, y);

			if (pixel1 == blackPixel)
			{
				if (y > 0)
				{
					if (x > 0)
					{
						if (screen.getPixel(x - 1, y - 1) == blackPixel) //if Up-Left pixel is black
						{
							screen.setPixel(x - 1, y, mixColors(pixel1, screen.getPixel(x - 1, y), 2)); //Left
						}
					}

					if (x < 159)
					{
						if (screen.getPixel(x + 1, y - 1) == blackPixel) //if Up-Right pixel is black
						{
							screen.setPixel(x + 1, y, mixColors(pixel1, screen.getPixel(x + 1, y), 2));; //Right
						}
					}

				}

				if (y < 143)
				{
					if (x > 0)
					{
						if (screen.getPixel(x - 1, y + 1) == blackPixel) //if Down-Left pixel is black
						{
							screen.setPixel(x - 1, y, mixColors(pixel1, screen.getPixel(x - 1, y), 2)); //Left
						}
					}

					if (x < 159)
					{
						if (screen.getPixel(x + 1, y + 1) == blackPixel) //if Down-Right pixel is black
						{
							screen.setPixel(x + 1, y, mixColors(pixel1, screen.getPixel(x + 1, y), 2)); //Right
						}
					}
				}
			}
		}
	}


	return screen;
}

Image rainbowFilter(Image screen)
{
	for (int x = 0; x < 160; x++)
	{
		for (int y = 0; y < 144; y++)
		{
			Color pixel1 = screen.getPixel(x, y);

			screen.setPixel(x, y, mixColors(pixel1, Color(rand() % 255 + 0, rand() % 255 + 0, rand() % 255 + 0, 0xFF), 3.5));
		}
	}
	return screen;
}

Image lcdFilter1(Image screen)
{

	for (int y = 0; y < 144; y += 2)
	{
		for (int x = 0; x < 160; x++)
		{
			screen.setPixel(x, y, mixColors(screen.getPixel(x, y), Color(50, 50, 50, 255)));
		}
	}

	return screen;
}


Image _2xSaI(Image screen)
{
	auto compareColors = [](auto color1, auto color2, auto color3, auto color4)
	{
		int a = 0, b = 0, c = 0;

		if (color1 == color3)
		{
			a++;
		}
		else if (color2 == color3)
		{
			b++;
		}

		if (color1 == color4)
		{
			a++;
		}
		else if (color2 == color4)
		{
			b++;
		}

		if (a <= 1)
		{
			c++;
		}

		if (b <= 1)
		{
			c--;
		}


		
		
		return c;
	};

	Image scaledScreen;
	scaledScreen.create(160 * 2, 144 * 2);

	Color a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;

	Color color1, color2, color3;

	for (int y = 0; y < 144; y++)
	{

		for (int x = 0; x < 160; x++)
		{
			if ((y < (144)))
			{
				/*
				A B C D
				E F G H
				I J K L
				M N O P

				B is current pixel
				*/

				if (x < 1)
				{
					a = screen.getPixel(x - 1, y);
					b = screen.getPixel(x, y);
					c = screen.getPixel(x + 1, y);
					d = screen.getPixel(x + 2, y);

					e = screen.getPixel(x - 1, y + 1);
					f = screen.getPixel(x, y + 1);
					g = screen.getPixel(x + 1, y + 1);
					h = screen.getPixel(x + 2, y + 1);

					i = screen.getPixel(x - 1, y + 2);
					j = screen.getPixel(x, y + 2);
					k = screen.getPixel(x + 1, y + 2);
					l = screen.getPixel(x + 2, y + 2);

					m = screen.getPixel(x - 1, y + 3);
					n = screen.getPixel(x, y + 3);
					o = screen.getPixel(x + 1, y + 3);
					p = screen.getPixel(x + 2, y + 3);
				}
				else
				{
					a = screen.getPixel(x, y);
					b = screen.getPixel(x, y);
					c = screen.getPixel(x + 1, y);
					d = screen.getPixel(x + 2, y);

					e = screen.getPixel(x, y + 1);
					f = screen.getPixel(x, y + 1);
					g = screen.getPixel(x + 1, y + 1);
					h = screen.getPixel(x + 2, y + 1);

					i = screen.getPixel(x, y + 2);
					j = screen.getPixel(x, y + 2);
					k = screen.getPixel(x + 1, y + 2);
					l = screen.getPixel(x + 2, y + 2);

					m = screen.getPixel(x, y + 3);
					n = screen.getPixel(x, y + 3);
					o = screen.getPixel(x + 1, y + 3);
					p = screen.getPixel(x + 2, y + 3);
				}


				if ((f == k) && (g != j))
				{
					if (((f == b) && (g == l)) || ((f == j) && (f == c) && (g != b) && (g == d)))
					{
						color1 = f;
					}
					else
					{
						color1 = cosineInterpolation(f, g, .5);
					}

					if (((f == e) && (j == o)) || ((f == g) && (f == i) && (e != j) && (j == m)))
					{
						color2 = f;
					}
					else
					{
						color2 = cosineInterpolation(f, j, .5);
					}

					color3 = f;

				}
				else if ((g == j) && (f != k))
				{
					if (((g == c) && (f == i)) || ((g == b) && (g == k) && (f != c) && (h == i)))
					{
						color1 = g;
					}
					else
					{
						color1 = cosineInterpolation(f, g, .5);
					}

					if (((j == i) && (f == c)) || ((j == e) && (j == k) && (f != i) && (f == a)))
					{
						color2 = j;
					}
					else
					{
						color2 = cosineInterpolation(f, j, .5);
					}

					color3 = g;
				}
				else if ((f == k) && (g == j))
				{

					if (f == g)
					{
						color1 = f;
						color2 = f;
						color3 = f;
					}
					else
					{
						color1 = cosineInterpolation(f, j, .5);
						color2 = cosineInterpolation(f, g, .5);

						int compareValue = 0;

						compareValue += compareColors(f, g, e, b);
						compareValue += compareColors(g, f, h, c);
						compareValue += compareColors(g, f, i, n);
						compareValue += compareColors(f, g, l, o);

						if (compareValue > 0)
						{
							color3 = f;
						}
						else if (compareValue < 0)
						{
							color3 = g;
						}
						else
						{
							color3 = cosineInterpolation4(f, g, j, k, .5);
						}
					}

				}
				else
				{
					color3 = cosineInterpolation4(f, g, j, k, .5);

					if ((f == j) && (f == c) && (g != b) && (g == d))
					{
						color1 = f;
					}
					else if((g == b) && (g == k) && (f != c) && (a == f))
					{
						color1 = g;
					}
					else
					{
						color1 = cosineInterpolation(f , g, .5);
					}

					if ((f == g) && (f == i) && (e != j) && (j == m))
					{
						color2 = f;
					}
					else if ((j == e) && (j == k) && (f != i) && (a == f))
					{
						color2 = j;
					}
					else
					{
						color2 = cosineInterpolation(f, j, .5);
					}

				}

				scaledScreen.setPixel((x * 2), (y * 2), f);
				scaledScreen.setPixel((x * 2) + 1, (y * 2), color1);
				scaledScreen.setPixel((x * 2), (y * 2) + 1, color2);
				scaledScreen.setPixel((x * 2) + 1, (y * 2) + 1, color3);

			}

		}

	}

	return scaledScreen;
}

Image filterSelect(Image screen, byte filterNumber)
{

	if (filterNumber == 1)
	{
		return eagleFilter(screen);
	}
	if (filterNumber == 2)
	{
		return scale2XFilter(screen);
	}
	if (filterNumber == 3)
	{
		return scale3XFilter(screen);
	}
	if (filterNumber == 4)
	{
		return blurFilter(screen);
	}
	if (filterNumber == 5)
	{
		return customFilter(screen);
	}
	if (filterNumber == 6)
	{
		return zigZagFilter(screen);
	}
	if (filterNumber == 7)
	{
		return rainbowFilter(screen);
	}
	if (filterNumber == 8)
	{
		return lcdFilter1(screen);
	}
	if (filterNumber == 10)
	{
		return _2xSaI(screen);
	}
	else
	{
		return screen;
	}
}
