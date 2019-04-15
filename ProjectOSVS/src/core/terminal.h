#ifndef TERMINAL_H_GUARD
#define TERMINAL_H_GUARD
#include "vgautils.h"
//#include "memory.h"
namespace Core {
	using uint32_t = unsigned int;
	using uint16_t = unsigned short;
	using uint8_t = unsigned char;
	
	//strlen provisoire
	uint32_t strlen(const char* str)
	{
		uint32_t len = 0;
		while (str[len])
			len++;
		return len;
	}

	/*
		Terminal interface
		_CharType: type of displayed char
	*/
	template<class _CharType>class Terminal {
		uint32_t m_Row, m_Column, m_MaxWidth, m_MaxHeight;
		uint8_t m_Color;	//Color optained with vga_utils::entryColor
		uint16_t* m_Buffer; //Adresse of vga

		//Custom char: TODO: create a class to handle this
		enum CustomChar {
			COLOR,
			NONE
		};

		CustomChar m_CustomChActive;	//if a custom char is active
		int m_CustomCharSizeLeft;		//size left to capture custom char parameter
		_CharType* m_CustomCharArrayBuffer;	//array who contained folowed characters of custom char

		//add c at position x, y with color color
		void putEntryAt(const _CharType c, const uint8_t color, const uint32_t x, const uint32_t y) {
			const uint32_t i = y * m_MaxWidth + x;
			m_Buffer[i] = vga_utils::entry(c, color);
		}

	public:

		//add char c to terminal (Handled custom char)
		void putchar(const _CharType c) {
			if (m_CustomCharSizeLeft == -1) { //If character '/' is detected before
				switch (c)
				{
				case 'c': { //Color handling
					m_CustomCharSizeLeft = 2; //Color range: 00~15 so 2 characters required
					//m_CustomCharArrayBuffer = (_CharType*)malloc(2*sizeof(_CharType));
					m_CustomChActive = Core::Terminal<_CharType>::COLOR;
					return;
				}
				default: {}break;
				}
			}
			else if (m_CustomCharSizeLeft > 0) { //Custom char need to capture more characters after '/'
				--m_CustomCharSizeLeft;
				switch (m_CustomChActive)
				{
				case Core::Terminal<_CharType>::COLOR: {
					m_CustomCharArrayBuffer[m_CustomCharSizeLeft] = c; //Capturing character but in inverted order to simplify the function
					if (!m_CustomCharSizeLeft) {
						setColor((vga_utils::vga_color)((m_CustomCharArrayBuffer[1] - '0') * 10 + (m_CustomCharArrayBuffer[0] - '0'))); //Converting 2 chars to int:
						/* characters are inverted in array to optimize their capture so if /c12 is entered :
							m_CustomCharArrayBuffer[1] = 1
							m_CustomCharArrayBuffer[0] = 2

							and to get 10 we need to :
							m_CustomCharArrayBuffer[1] * 10 = 10
							10 + m_CustomCharArrayBuffer[2] = 12

							[TO OPTMIZE ?]
						*/
						//free(m_CustomCharArrayBuffer, 2 * sizeof(_CharType));
					}
					return;
				}
					break;
				case Core::Terminal<_CharType>::NONE:
					break;
				default:
					break;
				}
			}

			if (c == '\n') { //Carriage return
				++m_Row;
				m_Column = 0;
				return;
			}
			else if (c == '$') { //Custom char
				m_CustomCharSizeLeft = -1;
				return;
			}
			putEntryAt(c, m_Color, m_Column, m_Row); //Add the character
			if (++m_Column == m_MaxWidth) { //Out of range check
				m_Column = 0u;
				if (++m_Row == m_MaxHeight)
					m_Row = 0u;
			}
		}

	
		Terminal() : m_CustomChActive(NONE), m_CustomCharSizeLeft(0) {
		}

		//Initializing terminal (wipe + size)
		void init(const unsigned long width, const unsigned long height,
			const vga_utils::vga_color fontColor=vga_utils::VGA_COLOR_LIGHT_GREY,
			const vga_utils::vga_color backgroundColor=vga_utils::VGA_COLOR_BLACK) {
			m_MaxHeight = height;
			m_MaxWidth = width;
			m_Row = m_Column = 0u;
			m_Color = vga_utils::entryColor(fontColor, backgroundColor);
			m_Buffer = (uint16_t*)VGA_ADDR;
			for (uint32_t y(0u); y < m_MaxHeight; ++y) { //Filling columns and row of ' ' char to empty space
				for (uint32_t x(0u); x < m_MaxWidth; ++x) {
					const uint32_t i = y * m_MaxWidth + x;
					m_Buffer[i] = vga_utils::entry(' ', m_Color);
				}
			}
		}

		//Set color of following characters since another setColor
		void setColor(const vga_utils::vga_color fontColor = vga_utils::VGA_COLOR_LIGHT_GREY,
			const vga_utils::vga_color backgroundColor = vga_utils::VGA_COLOR_BLACK) {
			m_Color = vga_utils::entryColor(fontColor, backgroundColor);
		}

		//Add str to terminal
		void write(const _CharType* str) {
			write(str, strlen(str));
		}

		//Add str of size size to terminal
		void write(const _CharType* str, const uint32_t size) {
			for (uint32_t i(0u); i < size; ++i)
				putchar(str[i]);
		}
	};
}



#endif