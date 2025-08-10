#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <cstdio>
#include <cstdlib>

/// You can find the stb_truetype library at the link below: 
///
/// https://github.com/nothings/stb

int main() {
  /// In order to load a font with stb_truetype, we will first have to 
  /// load contents of the file into a buffer of `unsigned char`s.
  ///
  /// This buffer can then be passed to stb_truetype to be parsed for all 
  /// the neccessary information. The function will then fill the given `info` 
  /// variable of type `stbtt_fontinfo`. This structure will contain all the 
  /// relevant information about the font (as implied by the name). 
  ///
  /// Just make sure that the buffer you give to STB is actually valid. You 
  /// can free it after the load function has returned. The `stbtt_InitFont` 
  /// function will return `0` if it fails to load the font, and `1` if successful.
  ///
  /// Sadly, as far as I know stb_truetype does not include any API that takes a 
  /// file path and does the heavy-lifting for you. But, thankfully, reading 
  /// binary data from a file is simple enough. 

  stbtt_fontinfo info;
  unsigned char* font_data = read_file_in_bytes("path/to/font.ttf"); 

  /// The three parameters of the `stbtt_InitFont` are pretty simple to understand. 
  /// 
  /// The first parameter takes a point to a `stbtt_fontinfo` structure. It will 
  /// fill this structure if the function successfully returns. 
  ///
  /// The second parameter takes in the buffer of data you (hopefull) loaded from  
  /// the file.
  ///
  /// The third and final parameter is the "offset" into the font file. 
  ///
  /// For a single `.ttf` font, there is only one font embeded into the 
  /// file. However, some fonts (named a "font collection") come with 
  /// multiple fonts embeded into a singe file. These font collection   
  /// files have a `.ttc` extension. The offset parameter will dictate which 
  /// font to extract out of the font file. For most `.ttf` files, however, 
  /// the offset will always be `0`. But, if you want to extract a certain 
  /// font out of a font collection you can use the `stbtt_GetFontOffsetForIndex` 
  /// function to do so. 
  ///
  /// This function takes the font_data and then an index of the certain embeded 
  /// font you need the offset of. The function will return the offset that can then 
  /// be given to `stbtt_InitFont` or `-1` if the given index is out of range. 
  ///
  /// You can also use the `stbtt_GetNumberOfFonts` function to retrieve the maximum 
  /// number of fonts in a given font. 
  ///
  /// Once again, though, for most `.ttf` files, `0` is usually the valid offset.

  int num_fonts = stbtt_GetNumberOfFonts(font_data); 
  int offset    = stbtt_GetFontOffsetForIndex(font_data, 0);

  if(stbtt_InitFont(&info, font_data, offset) == 0) {
    printf("ERROR: Could not initialize STB truetype library!\n"); // Always check for errors!
    return -1;
  }

  /// The `stbtt_ScaleForPixelHeight` function will return a scaling factor 
  /// that can be applied to any value that comes out of stb_truetype going forward. 
  ///
  /// If you do not set the scaling factor, the "quality" of the fonts you load 
  /// will differ from font to font. However, if you use this function and multiply it with
  /// any variable that comes out of stb_truetype, you'll bring these variables to a 
  /// "scaled" coordinate system with a more "deterministic" outcome. 
  ///
  /// Improtant to note that this `scale_factor` variable will also be required 
  /// for any subsequent function that will query some information about each glyph. 
  ///
  /// The bigger the value the more "crips" your fonts will be. But, as far as I can tell, 
  /// `256.0f` is the value that is good for this scaling factor. Anything more than that 
  /// will not yield any resutls, and anything less is pixely. You can use any value you want, though. 
  /// It depends on your needs. 
  ///
  /// As you can see, the function takes in the font info struct and then the scaling factor in pixels.

  float scale_factor = stbtt_ScaleForPixelHeight(info, 256.0f);

  /// This function will get the vertical metrics and/or information about the given font. 
  /// This is the information that gets computed between each row of the font. 
  ///
  /// The function will return the ascent, descent, and the line gap of the font. 
  ///
  /// The ascent defines the very top of the font (it's usually positive). 
  /// The descent defines the very bottom of the font (it's usually negative). 
  /// The line gap defines the spacing between one row's ascent and the next 
  /// row's ascent.

  int ascent, descent, line_gap;
  stbtt_GetFontVMetrics(info, &ascent, &descent, &line_gap);

  /// Bringing our newly filled variables into a "scaled" coordinate space.
  /// If we don't do this, the variables will have a _huge_ value that will 
  /// not map well when rendering. It will work fine if you're not using a 
  /// scaling factor at all. Otherwise, you need to do this.

  ascent   *= scale_factor; 
  descent  *= scale_factor; 
  line_gap *= scale_factor; 

  /// You can think of a font as an array of glyphs. You'll need to go 
  /// through each glyph and retrieve certain information about it 
  /// in order to render it to the screen. To retrieve a glyph in stb_truetype, 
  /// though, you can use two methods. 
  ///
  /// You can retrieve the actual _glyph_'s index and get the information about it.
  /// You an also retrieve the codepoint and get the information about that. 
  ///
  /// The difference between a "glyph" and a "codepoint" is that a "glyph" is just a bunch
  /// of the metrics like we're going to see in a moment. It's often more effecient to retrieve 
  /// the glyph than the codepoint, since a "codepoint" is just the actual ASCII representation 
  /// of the character. So the character 'A' is a codepoint. 
  ///
  /// The functions below have two types. Ones that operate on codepoints and ones that operate 
  /// on glyphs. Once again, it's often more effecient to operate on glyphs than codepoints. 
  ///
  /// To operate on glyphs, though, we first have to retrieve the index of that glyph in the font. 
  /// stb_truetype has a helper function that does just that. The `stbtt_FindGlyphIndex` will take 
  /// the font info as well as the codepoint. It will then perform a look in the font and return 
  /// the appropriate index. That you can use with any querying functions that operate on glyphs.
  ///
  /// The function will also return a `0` if the codepoint is not found in the font. Make sure to 
  /// check for that, since you don't want to create a texture of a non-existent character.

  int glyph_index = stbtt_FindGlyphIndex(info, 'A');

  /// A single glyph in the font is just a greyscale texture that has a bunch information 
  /// describing its width, height, and so on. Keep in mind that _every_ glyph has _only_ 
  /// one byte per pixel or 8 bits per pixel. 
  ///
  /// The function below will allocate a big enough bitmap and return back a few useful 
  /// variables about the newly-allocated bitmap. The function also expects a scaling factor 
  /// for both the width and height. For us, though, we only care about the scaling factor of 
  /// the height since that's what we have thanks to the `stbtt_ScaleForPixelHeight`. You can 
  /// set the scaling factor for the width if you wish, thohg. 
  ///
  /// Besides the allocated bitmap, the function will return the width, height, 
  /// the X offset, and the Y offset. 
  ///
  /// The width and height is just the size of the bitmap. The offsets are a TTF 
  /// concept. Essentially, they are offset into the actual relavent part of the texture. 
  /// In some other APIs, this is called the "bearing" of the glyph.
  ///
  /// If you're having a problem understanding, you can check out this image from LearnOpenGL to get a better idea. 
  ///
  /// https://learnopengl.com/img/in-practice/glyph.png
  
  int width, height, offset_x, offset_y; 
  unsigned char* glyph_bitmap = stbtt_GetGlyphBitmap(info,         // Font info
                                                     0,            // Scale width 
                                                     scale_factor, // Scale height
                                                     glyph_index,  // Glyph index
                                                     &width,       // Out width
                                                     &height,      // Out height
                                                     &offset_x,    // Out offset X
                                                     &offset_y);   // Out offset Y

  /// After allocating the bitmap you can query it for different information. 
  /// For example, you can retrieve the glyph's bounding box centered around the origin, 
  /// using the function below.  
  ///
  /// It's very similiar to `stbtt_GetGlyphBitmap`. 
  ///
  /// The bounding box can be retrieved for various reasons. However, personally, I haven't 
  /// found any reason to use it. You can perhaps query stb_truetype for the bounding box 
  /// in order to retrieve the width and height to allocate a buffer and then pass into 
  /// the `stbtt_MakeGlyphBitmap` function. I'm sure there are other uses, though.
  ///
  /// Either way, the function will return the left, top, right, and bottom values of 
  /// the bounding box. You can actually calculate the width and height from these values. 
  /// 
  /// width  = left + right 
  /// height = top + bottom 
  ///
  /// Besides that, much like the `stbtt_GetGlyphBitmap`, the function also expects the scaling 
  /// factor for both the width and the height

  int left, top, right, bottom; 
  stbtt_GetGlyphBitmapBox(info,         // Font info
                          glyph_index,  // Glyph index
                          0,            // Scale width
                          scale_factor, // Scale height
                          &left,        // Left
                          &top,         // Top
                          &right,       // Right
                          &bottom);     // Bottom
   

  /// Besides getting back an allocated bitmap from stb_truetype, you can also 
  /// allocate your own buffer and pass it into stb_truetype in order to fill it. 
  /// This is a great method if you have your own allocated or you don't want STB to 
  /// allocate memory for you. However, it's a bit more work. 
  ///
  /// Firstly, you have to allocate your own buffer. The size of that buffer can be calculated 
  /// from the width and height of the glyph. We can retrieve that by using the `stbtt_GetGlyphBitmapBox` function. 
  /// That way, we will have both the width and height of the glyph and therefore we will have the total size. 
  /// And, remember, `stbtt_GetGlyphBitmapBox` retrieves the bounding box of the glyph _centered around the origin_. 
  /// Therefore, we have to calculate the width and height differently. 
  ///
  /// The `stbtt_MakeGlyphBitmap` will require you to pass in a pointer to the buffer you allocated, 
  /// the width and height of the glyph, the stride or the spacing between each row of the font, 
  /// the scaling factor for both the width and the height, and then the glyph index. 
  ///
  /// The stride is potentially the most complex out of all of these. As I said, it's the 
  /// spacing between each row of the font in bytes. We can use the height of each glyph 
  /// in order to set that. 

  int buff_width  = right - left;
  int buff_height = bottom - top; 
  int buff_size   = buff_width * buff_height; 

  unsigned char* out_bitmap = malloc(buff_size);

  stbtt_MakeGlyphBitmap(info,         // Font info
                        out_bitmap,   // The output buffer
                        buff_width,   // Width of the bitmap
                        buff_height,  // Height of the bitmap
                        buff_height,  // The 'stride' or the spacing between each row
                        scale_factor, // Scale width
                        scale_factor, // Scale height
                        glyph_index); // Glyph index

  /// Along with the vertical metrics of the font that we got earlier, we can 
  /// also retrieve the _horizontal_ metrics of each _glyph_. These metrics are 
  /// _extremely_ useful during rendering. Alongside the glyph's information, 
  /// we can use the horizontal metrix of the glyph to correctly place the glyph 
  /// on the screen. 
  ///
  /// We can use the `stbtt_GetGlyphHMetrics` to get that horizontal information. 
  /// As for the metrics themselves, there are two variables in particular. 
  /// 
  /// The _advance_ is the offset from the current horizontal position till 
  /// the _next_ horizontal position. Usually, this is called the "advance width" or 
  /// "advance x" in other APIs. 
  ///
  /// The *left_side_bearing* is the offset from the current horizontal position till the 
  /// left edge of the character. 
  ///
  /// For some fonts these values are very improtant since they will help to place 
  /// the glyph precisely at the correct position. However, for other fonts, these 
  /// values are usually very small or near 0. You _have_ to take them into account, 
  /// however, since you don't know which fonts will be used.
  
  int advance, left_side_bearing;
  stbtt_GetGlyphHMetrics(info, glyph_index, &advance, &left_side_bearing);

  /// Since the `stbtt_GetGlyphHMetrics` function does not take in the scaling factor, 
  /// we will have to bring the output variables into the "scaled" coordinates.

  advance           *= (int)scale_factor;
  left_side_bearing *= (int)scale_factor;

  /// A "kern" of a glyph is the spacing between to specific two glyphs. 
  /// You  can use this value when rendering in order to add extra spacing 
  /// between glyphs. 
  ///
  /// Furthermore, you have to call this function when _rendering_ to know 
  /// the _exact_ kern value between the two glyphs. The current glyph and the  
  /// next one to be rendered. This is often not practical since you might need to 
  /// keep the `stbtt_fontinfo` struct around. However, if you do wish to you 
  /// use this value, you'll have to pass in the index of the current glyph and 
  /// the index of the next glyph. 
  ///
  /// You can also use the codepoint version of this function, `stbtt_GetCodepointKernAdvance` 
  /// which takes in the actual codepoint values instead of a set of indices.

  int next_glyph_index = stbtt_FindGlyphIndex(info, 's');
  stbtt_GetGlyphKernAdvance(info, glyph_index, next_glyph_index);

  /// Once again, the kern is returned as in unscaled coordinates so we have 
  /// to bring it to scaled coordinates. 
  
  kern *= (int)scale_factor;

  /// There is also another way of retrieve the kern of each glyph. 
  /// Instead of querying the font to get the kern between two specific glyphs, 
  /// you can retrieve something called a "kern table", which is an array 
  /// of `stbtt_kerningentry`s.
  ///
  /// Each entry is structure like this: 
  /// 
  /// struct stbtt_kerningentry {
  ///   int glyph1; 
  ///   int glyph2; 
  ///
  ///   int advance;
  /// };
  ///
  /// You'll have to retrieve the length of this table first, though. And you 
  /// can easily do that by using the `stbtt_GetKerningTableLength` function. 
  ///
  /// You have to get the length of the table since you'll have to allocate it yourself. 
  /// Then, you can give the newly-allocated kerning table to the `stbtt_GetKerningTable` 
  /// function with the length of the kerning table that you retrieved.
  ///
  /// This is much more practical than querying the font every frame about the kern between two glyphs. 

  int kern_table_length = stbtt_GetKerningTableLength(info);

  stbtt_kerningentry* kern_table = (stbtt_kerningentry*)malloc(sizeof(stbtt_kerningentry) * kern_table_length);
  stbtt_GetKerningTable(info, kern_table);

  /// Since stb_truetype allocates the bitmap internally, we'll need to de-allocate 
  /// the bitmap as well using the function below. 
  ///
  /// The function takes in a pointer to the glyph bitmap and an extra user data 
  /// if you inserted any. We can leave that as null, though.

  stbtt_FreeBitmap(glyph_bitmap, nullptr);
}
