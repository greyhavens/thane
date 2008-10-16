////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.fonts;

import flash.swf.builder.types.PathIteratorWrapper;
import flash.swf.builder.types.ShapeBuilder;
import flash.swf.types.GlyphEntry;
import flash.swf.types.Shape;
import flash.swf.SwfConstants;
import flash.util.Trace;

import java.awt.font.FontRenderContext;
import java.awt.font.GlyphMetrics;
import java.awt.font.GlyphVector;
import java.awt.font.TextAttribute;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.BufferedInputStream;
import java.net.URL;
import java.util.Locale;
import java.util.Map;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * The <code>JREFontManager</code> attempts to optimize the loading of JDK fonts through caching
 * of <code>FontSet</code>s. A <code>FontSet</code> keeps track of the different styles (faces)
 * for a given font family as <code>FontFace</code> instances. A manager can derive
 * available styles from a single <code>FontFace</code> through its reference to a
 * <code>java.awt.Font</code> object.
 *
 * @author Peter Farland
 */
public class JREFontManager extends CachedFontManager
{
    private boolean readLocalFonts;
    private Map<String, LocalFont> localFonts;
    public static String LOCAL_FONTS_SNAPSHOT = "flex.fonts-snapshot";
    private String localFontsFile;

    public void initialize(Map map)
    {
        super.initialize(map);
        if (map != null)
        {
            localFontsFile = (String)map.get(LOCAL_FONTS_SNAPSHOT);
        }
        if (localFontsFile == null)
        {
            localFontsFile = "localFonts.ser";
        }
    }

    protected String createFontFromLocation(Object location, int style, boolean useTwips)
    {
        String family = null;
        InputStream is = null;

        try
        {
            if (location != null && location instanceof URL)
            {
                URL url = (URL)location;

                if (url.getProtocol().toLowerCase().indexOf("file") > -1)
                {
                    File f = new File(url.getFile());
                    // Do NOT buffer this stream, Font.createFont() does it for us.
                    is = new FileInputStream(f);
                }
                else
                {
                    is = url.openStream();
                }

                Font font = Font.createFont(Font.TRUETYPE_FONT, is);

                // Get the family and fontName info before we derive a new size as
                // the name might be reset to dialog!
                family = font.getFamily();
                String fontName = font.getFontName(Locale.ENGLISH);

                // We need to examine the name for the real style as
                // Font.createFont always sets style to PLAIN and the user may
                // have given us the wrong style info
                int realStyle = CachedFontFace.guessStyleFromSubFamilyName(fontName);

                // Only bother deriving a font and storing the work if this really was the style requested
                if (style == realStyle)
                {
                    // Now that we've processed the font with the JRE font manager, we now need to get the fsType
                    // and copyright info from from Batik.  This is a bit strange, as one of the advantages of the
                    // JRE font manager is that it can process some fonts that Batik cannot.  This check is needed,
                    // though, anytime we create a font.  So it may mean we don't need the JRE font manager anymore,
                    // but since I'm not sure of that, it'll be kept in for awhile.
                    String locationStr = BatikFontManager.processLocation(location);
                    org.apache.batik.svggen.font.Font batikFont = org.apache.batik.svggen.font.Font.create(locationStr);
                    if (batikFont == null)
                    {
                        throw new FontFormatException ("Unable to create font." );
                    }
                    FSType type = FSType.getFSType(batikFont);
                    if (! type.usableByFlex)
                    {
                        throw new BatikFontManager.UnusableFontLicense(location + "", type.description);
                    }
                    String copyright = batikFont.getNameTable().getRecord((short)0);
                    String trademark = batikFont.getNameTable().getRecord((short)7);

                    // Set to a default size so metrics are established
                    // getAttributes() returns a Map which is based on a Hashtable of <Obj,Obj>, or <TA,Obj> in practice
                    // we need to suppress the cast because getAttributes defines the return type as <TA,?> 
                    @SuppressWarnings("unchecked")
                    Map<TextAttribute, Object> attributes = (Map<TextAttribute, Object>) font.getAttributes();

                    attributes.put(TextAttribute.FAMILY, family);
                    attributes.put(TextAttribute.SIZE, DEFAULT_FONT_SIZE_OBJECT);
                    attributes.put(TextAttribute.POSTURE, CachedFontFace.isItalic(realStyle) ? TextAttribute.POSTURE_OBLIQUE : TextAttribute.POSTURE_REGULAR);
                    attributes.put(TextAttribute.WEIGHT, CachedFontFace.isBold(realStyle) ? TextAttribute.WEIGHT_BOLD : TextAttribute.WEIGHT_REGULAR);

                    font = font.deriveFont(attributes);

                    FontSet fontSet = (FontSet)getFontCache().get(family);
                    if (fontSet == null)
                    {
                        fontSet = new FontSet(maxFacesPerFont);
                        getFontCache().put(family, fontSet);
                    }

                    fontSet.put(style, new JREFontFace(font, realStyle, maxGlyphsPerFace, type, copyright, trademark, useTwips));
                }
            }
        }
        catch (FileNotFoundException ex)
        {
            return null;
        }
        catch (FontFormatException ex)
        {
            return null;
        }
        catch (IOException ex)
        {
            return null;
        }
        finally
        {
            try
            {
                is.close();
            }
            catch (Throwable t)
            {
            }
        }

        return family;
    }

    /**
     * Attempts to load a font from the cache by location or from disk if it is the first
     * request at this address. The location is bound to a font family name after the initial
     * loading, and the relationship exists for the lifetime of the cache.
     *
     * @param location
     * @param style
     * @return FontSet.FontFace
     */
    public FontFace getEntryFromLocation(URL location, int style, boolean useTwips)
    {
        FontFace entry = null;
        Object fam = getFontFileCache().get(location);

        if (fam == null)
        {
            fam = createFontFromLocation(location, style, useTwips);
        }

        if (fam != null)
        {
            String family = fam.toString();

            FontSet fontSet = (FontSet)getFontCache().get(family);

            // The font file cache should still have this family
            // from the location fetch above...
            if (fontSet != null)
            {
                entry = fontSet.get(style);
            }
        }

        return entry;
    }

    private static ArrayList<String> systemFontNames = null;

    protected FontSet createSetForSystemFont(String family, int style, boolean useTwips)
    {
        FontSet fontSet = null;
        if (family != null)
        {
            if (systemFontNames == null)
                initializeSystemFonts();

            if (systemFontNames != null && !systemFontNames.contains(family.trim().toLowerCase()))
            {
                if (Trace.font)
                    Trace.trace("Font family '" + family + "' not known to JRE.");

                return null;
            }

            //Load a font by family and style, set size to 240 for greater granularity
            Font font = Font.decode(family + "-" + getStyleAsString(style) + "-" + DEFAULT_FONT_SIZE_STRING);

            fontSet = new FontSet(maxFacesPerFont);
            fontSet.put(font.getStyle(), new JREFontFace(font, font.getStyle(), maxGlyphsPerFace, null, null, null, useTwips));
        }
        return fontSet;
    }

    /**
     * Attempts to locate a font by family name and style from the JRE's list of
     * fonts, which are primarily system registered fonts.
     *
     * @param familyName
     * @param style      - either Font.PLAIN, Font.BOLD, Font.ITALIC or Font.BOLD+Font.ITALIC
     * @return FontSet.FontFace
     */
    public FontFace getEntryFromSystem(String familyName, int style, boolean useTwips)
    {
        if (! readLocalFonts)
          {
              readLocalFonts();
          }

        FontFace entry = null;

        FontSet fontSet = (FontSet)getFontCache().get(familyName);

        // This is likely to be the first time looking for this family
        if (fontSet == null)
        {
            fontSet = createSetForSystemFont(familyName, style, useTwips);
        }

        // If the family was invalid on the OS there's nothing more we can do here
        if (fontSet != null)
        {
            entry = fontSet.get(style);
        }

        if (entry != null)
        {
            LocalFont font = localFonts.get(entry.getPostscriptName());
            if (font == null)
            {
                // silent failure
                if (Trace.font)
                {
                    Trace.trace("Information on font " + familyName + " could not be found.  Run FontSnapshot to get a list of the current local fonts.");
                }
            }
            else
            {
                entry.setCopyright(font.copyright);
                entry.setTrademark(font.trademark);
                entry.setFSType(FSType.getFSType(font.fsType));
            }
        }

        return entry;
    }

	private void readLocalFonts()
    {
        readLocalFonts = true;

        localFonts = new HashMap<String, LocalFont>();
//		initDefaultLocalFonts();
        try
        {
            InputStream buffStream = new BufferedInputStream(new FileInputStream(localFontsFile));
            ObjectInputStream in = new ObjectInputStream(buffStream);
            
            // there's no way around suppressing the cast warning from Object, the types get erased anyway
            @SuppressWarnings("unchecked")
            Map<String, LocalFont> customLocalFonts = (Map<String, LocalFont>)in.readObject();
            
            localFonts.putAll(customLocalFonts);
        }
        catch(FileNotFoundException fnfe)
        {
            // ignore... a message will be printed out later if needed
            if (Trace.font)
            {
                fnfe.printStackTrace();
            }
        }
        catch(Exception fnfe)
        {
            if (Trace.font)
            {
                Trace.trace("Could not read localFonts.ser: " + fnfe);
            }
        }
    }

    private String getStyleAsString(int style)
    {
        String styleName;

        switch (style)
        {
            case 1:
                styleName = "bold";
                break;
            case 2:
                styleName = "italic";
                break;
            case 3:
                styleName = "bolditalic";
                break;
            default:
                styleName = "plain";
        }

        return styleName;
    }

    private static void initializeSystemFonts()
    {
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        String[] fnts = ge.getAvailableFontFamilyNames();

        if (fnts != null)
        {
            systemFontNames = new ArrayList<String>(fnts.length);
            for (int i = 0; i < fnts.length; i++)
            {
                systemFontNames.add(fnts[i].trim().toLowerCase());
            }
        }
    }

    public static class JREFontFace extends CachedFontFace
    {
	    // FIXME: need to deal with useTwips differently for caching

        public JREFontFace(Font font, int style, int maxGlyph, FSType fsType, String copyright, String trademark, boolean useTwips)
        {
            super(maxGlyph, style, fsType, copyright, trademark, useTwips);
            this.font = font;
            init();
        }

        private void init()
        {
            //Convert from device to grid co-ordinates, fixed at 72dpi
            emScale = SWF_EM_SQUARE / getPointSize(); //If you want to correct for resolution, multiply this value by 72/resolution

            scaleTransform = new AffineTransform();
            scaleTransform.setToScale(emScale, emScale);

            //We use a BufferedImage to get to the system FontMetrics...
            //Feel free to suggest a better way of getting this object.
            if (graphics == null)
            {
                BufferedImage bi = new BufferedImage(50, 50, BufferedImage.TYPE_INT_RGB);
                graphics = bi.createGraphics();
                graphics.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            }

            fontMetrics = graphics.getFontMetrics(font);
            frc = new FontRenderContext(null, true, false);

            ascent = (int)Math.rint(fontMetrics.getAscent() * emScale * (useTwips ? SwfConstants.TWIPS_PER_PIXEL : 1));
            descent = (int)Math.rint(fontMetrics.getDescent() * emScale * (useTwips ? SwfConstants.TWIPS_PER_PIXEL : 1));

            //Ignore JDK leading values, they never match Flash MX - using an estimation technique similar to Breeze instead.
            //leading = (int)StrictMath.rint(fontMetrics.getLeading() * emScale);
            //if (leading == 0)
            lineGap = (int)Math.rint(Math.abs((getPointSize() - ascent - descent)));
        }

        public boolean canDisplay(char c)
        {
            return font.canDisplay(c);
        }

        public String getFamily()
        {
            return font.getName();
        }

        public int getFirstChar()
        {
            return 0;
        }

        public GlyphEntry getGlyphEntry(char c)
        {
            return (GlyphEntry)glyphCache.get(c);
        }

        protected GlyphEntry createGlyphEntry(char c)
        {
            return createGlyphEntry(c, c);
        }

        public GlyphEntry createGlyphEntry(char c, char referenceChar)
        {
            Shape swfShape = null;
            int advance = 0;

            GlyphVector gv = font.createGlyphVector(frc, new char[]{referenceChar});
            java.awt.Shape glyphOutline = gv.getGlyphOutline(0);

            GlyphMetrics metrics = gv.getGlyphMetrics(0);
            advance = (int)Math.rint(metrics.getAdvance()); //Do not scale here, DefineText needs values unscaled

            java.awt.Shape scaledShape = scaleTransform.createTransformedShape(glyphOutline);
            swfShape = createGlyphShape(scaledShape);

            GlyphEntry ge = new GlyphEntry();
            ge = new GlyphEntry(); //Note: we will set the index on building DefineFont2 tag
            ge.advance = (int)(advance * emScale * (useTwips ? SwfConstants.TWIPS_PER_PIXEL : 1));
            ge.shape = swfShape;
	        //Glyph bounds are not used by the Flash Player so no need to calculate
	        //Rectangle2D bs = scaledShape.getBounds2D();
	        //bounds = new Rect((int)StrictMath.rint(bs.getMinX() * (useTwips ? SwfConstants.TWIPS_PER_PIXEL : 1)),
			//        (int)StrictMath.rint(bs.getMaxX() * (useTwips ? SwfConstants.TWIPS_PER_PIXEL : 1)),
			//        (int)StrictMath.rint(bs.getMinY() * (useTwips ? SwfConstants.TWIPS_PER_PIXEL : 1)),
			//        (int)StrictMath.rint(bs.getMaxY()) * (useTwips ? SwfConstants.TWIPS_PER_PIXEL : 1));
            //ge.bounds = bounds;
            ge.character = c;

            return ge;
        }

        private Shape createGlyphShape(java.awt.Shape outline)
        {
            ShapeBuilder shape = new ShapeBuilder(useTwips);
            shape.setCurrentLineStyle(0);
            shape.setCurrentFillStyle1(1);
            shape.setUseFillStyle1(true);
            shape.processShape(new PathIteratorWrapper(outline.getPathIterator(null)));

            return shape.build();
        }

        public int getAdvance(char c)
        {
            return 0;  //To change body of implemented methods use File | Settings | File Templates.
        }

        public Font getFont()
        {
            return font;
        }

        public int getMissingGlyphCode()
        {
            return font.getMissingGlyphCode();
        }

        public double getPointSize()
        {
            return font.getSize2D();
        }

        public FontRenderContext getFontRenderContext()
        {
            return frc;
        }

        public int getAscent()
        {
            return ascent;
        }

        public int getDescent()
        {
            return descent;
        }

        public int getLineGap()
        {
            return lineGap;
        }

        public int getNumGlyphs()
        {
            return font.getNumGlyphs();
        }

        public double getEmScale()
        {
            return emScale;
        }

        public String getPostscriptName()
        {
            return font.getPSName();
        }

        private Font font;
        private static Graphics2D graphics;
        private FontRenderContext frc;
        private FontMetrics fontMetrics;
        private int ascent;
        private int descent;
        private int lineGap;
        private double emScale;
        private AffineTransform scaleTransform;
    }

    // see FontSnapshot for adding more fonts to this list
    /*private static void initDefaultLocalFonts()
     {
         // map items found via FontSnapshot
         localFonts.put("ArialMT", new LocalFont("ArialMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Arial® Trademark of The Monotype Corporation plc registered in the US Pat & TM Off. and elsewhere."));
         localFonts.put("Arial-BoldMT", new LocalFont("Arial-BoldMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Arial® Trademark of The Monotype Corporation plc registered in the US Pat & TM Off. and elsewhere."));
         localFonts.put("Arial-BoldItalicMT", new LocalFont("Arial-BoldItalicMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Arial® Trademark of The Monotype Corporation plc registered in the US Pat & TM Off. and elsewhere."));
         localFonts.put("Arial-ItalicMT", new LocalFont("Arial-ItalicMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Arial® Trademark of The Monotype Corporation plc registered in the US Pat & TM Off. and elsewhere."));
         localFonts.put("ArialNarrow", new LocalFont("ArialNarrow", null, 0, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc/Type Solutions Inc.1990-1991 All Rights Reserved.", "Arial is a registered trademark of The Monotype Corporation plc."));
         localFonts.put("ArialNarrow-Bold", new LocalFont("ArialNarrow-Bold", null, 0, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc / Type Solutions Inc. 1990-91 All Rights Reserved.", "Arial is a registered trademark of The Monotype Corporation plc."));
         localFonts.put("ArialNarrow-BoldItalic", new LocalFont("ArialNarrow-BoldItalic", null, 0, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc / Type Solutions Inc. 1990-91 All Rights Reserved.", "Arial is a registered trademark of The Monotype Corporation plc."));
         localFonts.put("ArialNarrow-Italic", new LocalFont("ArialNarrow-Italic", null, 0, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc / Type Solutions Inc. 19-1991 All Rights Reserved.", "Arial is a registered trademark of The Monotype Corporation plc."));
         localFonts.put("Arial-Black", new LocalFont("Arial-Black", null, 0, "Digitized data copyright The Monotype Corporation 1991-1995. All rights reserved. Arial? is a trademark of The Monotype Corporation which may be registered in certain jurisdictions.", "Arial? is a trademark of The Monotype Corporation which may be registered in certain jurisdictions."));
         localFonts.put("BellMT", new LocalFont("BellMT", null, 8, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc, Type Solutions Inc 1990-1992. All rights reserved.", "Bell ? Trademark of The Monotype Corporation plc registered in the US Pat & TM Off."));
         localFonts.put("BellMTBold", new LocalFont("BellMTBold", null, 8, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc, Type Solutions inc 1990-1992. All rights reserved. Portions ? 1992 Microsoft Corp.", "Bell ? Trademark of The Monotype Corporation plc registered in the US Pat & TM Off."));
         localFonts.put("BellMTItalic", new LocalFont("BellMTItalic", null, 8, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc, Type Solutions Inc 1990-1992. All rights reserved. Portions ? 1992 Microsoft Corp.", "Bell ? Trademark of The Monotype Corporation plc registered in the US Pat & TM Off."));
         localFonts.put("Centaur", new LocalFont("Centaur", null, 8, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc/Type Solutions Inc. 1990-91-92 All Rights Reserved. Portions ? 1992 Microsoft Corp.", "Centaur ? Trademark of The Monotype Corporation plc registered in the US Pat & TM Off."));
         localFonts.put("Century", new LocalFont("Century", null, 0, "Digitized data copyright (C) 1992-1997 The Monotype Corporation. All rights reserved. Century? is a trademark of The Monotype Corporation which may be registered in certain jurisdictions.", "Century? is a trademark of The Monotype Corporation which may be registered in certain jurisdictions."));
         localFonts.put("ComicSansMS", new LocalFont("ComicSansMS", null, 0, "Copyright (c) 1995 Microsoft Corporation. All rights reserved.", "Comic Sans is a trademark of Microsoft Corporation."));
         localFonts.put("ComicSansMS-Bold", new LocalFont("ComicSansMS-Bold", null, 0, "Copyright (c) 1995 Microsoft Corporation. All rights reserved.", "Comic Sans is a trademark of Microsoft Corporation."));
         localFonts.put("CourierNewPSMT", new LocalFont("CourierNewPSMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1994. All Rights Reserved", "Courier™ Trademark of The Monotype Corporation plc registered in certain countries."));
         localFonts.put("CourierNewPS-BoldMT", new LocalFont("CourierNewPS-BoldMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Courier™ Trademark of The Monotype Corporation plc registered in certain countries."));
         localFonts.put("CourierNewPS-BoldItalicMT", new LocalFont("CourierNewPS-BoldItalicMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Courier™ Trademark of The Monotype Corporation plc registered in certain countries."));
         localFonts.put("CourierNewPS-ItalicMT", new LocalFont("CourierNewPS-ItalicMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Courier™ Trademark of The Monotype Corporation plc registered in certain countries."));
         localFonts.put("Garamond", new LocalFont("Garamond", null, 8, "Digitized data copyright Monotype Typography, Ltd 1991-1995. All rights reserved. Monotype Garamond? is a trademark of Monotype Typography, Ltd which may be registered in certain jurisdictions.", "Monotype Garamond? is a trademark of Monotype Typography, Ltd which may be registered in certain jurisdictions."));
         localFonts.put("Garamond-Bold", new LocalFont("Garamond-Bold", null, 8, "Digitized data copyright Monotype Typography, Ltd 1991-1995. All rights reserved. Monotype Garamond? is a trademark of Monotype Typography, Ltd which may be registered in certain jurisdictions.", "Monotype Garamond? is a trademark of Monotype Typography, Ltd which may be registered in certain jurisdictions."));
         localFonts.put("Garamond-Italic", new LocalFont("Garamond-Italic", null, 8, "Digitized data copyright Monotype Typography, Ltd 1991-1995. All rights reserved. Monotype Garamond? is a trademark of Monotype Typography, Ltd which may be registered in certain jurisdictions.", "Monotype Garamond? is a trademark of Monotype Typography, Ltd which may be registered in certain jurisdictions."));
         localFonts.put("Georgia", new LocalFont("Georgia", null, 0, "Typeface and data copyright 1996 Microsoft Corporation. All Rights Reserved.", "Georgia is a trademark of Microsoft Corporation."));
         localFonts.put("Georgia-Bold", new LocalFont("Georgia-Bold", null, 0, "Typeface and data copyright 1996 Microsoft Corporation. All Rights Reserved.", "Georgia is a trademark of Microsoft Corporation."));
         localFonts.put("Georgia-Italic", new LocalFont("Georgia-Italic", null, 0, "Typeface and data copyright 1996 Microsoft Corporation. All Rights Reserved.", "Georgia is a trademark of Microsoft Corporation."));
         localFonts.put("Georgia-BoldItalic", new LocalFont("Georgia-BoldItalic", null, 0, "Typeface and data copyright 1996 Microsoft Corporation. All Rights Reserved.", "Georgia is a trademark of Microsoft Corporation."));
         localFonts.put("CenturyGothic", new LocalFont("CenturyGothic", null, 0, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc / Type Solutions Inc. 1990-91 All Rights Reserved", "Century Gothic is a trademark of The Monotype Corporation plc."));
         localFonts.put("CenturyGothic-Bold", new LocalFont("CenturyGothic-Bold", null, 0, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc / Type Solutions Inc. 1990-91 All Rights Reserved", "Century Gothic is a trademark of The Monotype Corporation plc."));
         localFonts.put("CenturyGothic-BoldItalic", new LocalFont("CenturyGothic-BoldItalic", null, 0, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc / Type Solutions Inc. 1990-91 All Rights Reserved", "Century Gothic is a trademark of The Monotype Corporation plc."));
         localFonts.put("CenturyGothic-Italic", new LocalFont("CenturyGothic-Italic", null, 0, "Typeface ? The Monotype Corporation plc. Data ? The Monotype Corporation plc / Type Solutions Inc. 1990-91 All Rights Reserved", "Century Gothic is a trademark of The Monotype Corporation plc."));
         localFonts.put("LucidaBright", new LocalFont("LucidaBright", null, 8, "? 1991 Bigelow & Holmes Inc. Pat. Des. 289,421. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaBright-Demi", new LocalFont("LucidaBright-Demi", null, 8, "? 1991 by Bigelow & Holmes Inc. Pat. Des. 289,422. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaBright-DemiItalic", new LocalFont("LucidaBright-DemiItalic", null, 8, "? 1991 Bigelow & Holmes Inc. Pat. Des. 289,773. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaBright-Italic", new LocalFont("LucidaBright-Italic", null, 8, "? 1991 Bigelow & Holmes Inc. Pat. Des. 289,773. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaCalligraphy-Italic", new LocalFont("LucidaCalligraphy-Italic", null, 8, "? 1991 Bigelow & Holmes. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaSans", new LocalFont("LucidaSans", null, 0, "? 1991 Bigelow & Holmes Inc. Pat. Des. 289,420. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaSans-Demi", new LocalFont("LucidaSans-Demi", null, 0, "? 1991 Bigelow & Holmes Inc. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaSans-DemiItalic", new LocalFont("LucidaSans-DemiItalic", null, 0, "? 1991 by Bigelow & Holmes Inc. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaSans-Italic", new LocalFont("LucidaSans-Italic", null, 0, "? 1991 Bigelow & Holmes Inc. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Lucida? is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaConsole", new LocalFont("LucidaConsole", null, 8, "Copyright ? 1993 Bigelow & Holmes Inc. All rights reserved.", "Lucida is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("LucidaSansUnicode", new LocalFont("LucidaSansUnicode", null, 8, "Copyright ? 1993 Bigelow & Holmes Inc. All rights reserved. Pat. Des. 289,420. Pats. Pend.", "Lucida is a registered trademark of Bigelow & Holmes Inc."));
         localFonts.put("PalatinoLinotype-Roman", new LocalFont("PalatinoLinotype-Roman", null, 8, "Copyright 1981-1983, 1989,1993, 1998 Heidelberger Druckmaschinen AG. All rights reserved. The digitally encoded machine readable outline data for producing the Typefaces licensed are the property of Heidelberger Druckmaschinen AG and/or its subsidiaries, represented by Linotype Library GmbH, Dupont Strasse 1, 61352 Bad Homburg Germany. Portions © 1996-1998 Microsoft Corporation. All Rights Reserved. ", "Palatino® is a registered trademark of Heidelberger Druckmaschinen AG and its subsidiaries."));
         localFonts.put("PalatinoLinotype-Bold", new LocalFont("PalatinoLinotype-Bold", null, 8, "Copyright 1981-1983, 1989,1993, 1998 Heidelberger Druckmaschinen AG. All rights reserved. The  digitally encoded machine readable outline data for producing the Typefaces licensed are the property of Heidelberger Druckmaschinen AG and/or its subsidiaries, represented by Linotype Library GmbH, Dupont Strasse 1, 61352 Bad Homburg Germany. Portions © 1996-1998 Microsoft Corporation. All Rights Reserved.", "Palatino® is a registered trademark of Heidelberger Druckmaschinen AG and its subsidiaries."));
         localFonts.put("PalatinoLinotype-BoldItalic", new LocalFont("PalatinoLinotype-BoldItalic", null, 8, "Copyright 1981-1983, 1989,1993, 1998 Heidelberger Druckmaschinen AG. All rights reserved. The  digitally encoded machine readable outline data for producing the Typefaces licensed are the property of Heidelberger Druckmaschinen AG and/or its subsidiaries, represented by Linotype Library GmbH, Dupont Strasse 1, 61352 Bad Homburg Germany. Portions © 1996-1998 Microsoft Corporation. All Rights Reserved. ", "Palatino® is a registered trademark of Heidelberger Druckmaschinen AG and its subsidiaries."));
         localFonts.put("PalatinoLinotype-Italic", new LocalFont("PalatinoLinotype-Italic", null, 8, "Copyright 1981-1983, 1989,1993, 1998 Heidelberger Druckmaschinen AG. All rights reserved. The  digitally encoded machine readable outline data for producing the Typefaces licensed are the property of Heidelberger Druckmaschinen AG and/or its subsidiaries, represented by Linotype Library GmbH, Dupont Strasse 1, 61352 Bad Homburg Germany. Portions © 1996-1998 Microsoft Corporation. All Rights Reserved. ", "Palatino® is a registered trademark of Heidelberger Druckmaschinen AG and its subsidiaries."));
         localFonts.put("Tahoma", new LocalFont("Tahoma", null, 8, "? 2004 Microsoft Corporation. All rights reserved.", "Tahoma is a trademark of Microsoft Corporation."));
         localFonts.put("Tahoma-Bold", new LocalFont("Tahoma-Bold", null, 8, "? 2004 Microsoft Corporation. All rights reserved.", "Tahoma is a trademark of Microsoft Corporation."));
         localFonts.put("TempusSansITC", new LocalFont("TempusSansITC", null, 8, "Copyright (c) International Typeface Corporation 1996.  Portions Copyright (c) Microsoft Corporation 1996.  All rights reserved.", "ITC Tempus Sans is a Trademark of International Typeface Corporation."));
         localFonts.put("TimesNewRomanPSMT", new LocalFont("TimesNewRomanPSMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Times New Roman® Trademark of The Monotype Corporation plc registered in the US Pat & TM Off. and elsewhere."));
         localFonts.put("TimesNewRomanPS-BoldMT", new LocalFont("TimesNewRomanPS-BoldMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Times New Roman® Trademark of The Monotype Corporation plc registered in the US Pat & TM Off. and elsewhere."));
         localFonts.put("TimesNewRomanPS-BoldItalicMT", new LocalFont("TimesNewRomanPS-BoldItalicMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Times New Roman® Trademark of the Monotype Corporation plc registered in the US Pat & TM Off. and elsewhere."));
         localFonts.put("TimesNewRomanPS-ItalicMT", new LocalFont("TimesNewRomanPS-ItalicMT", null, 0, "Typeface © The Monotype Corporation plc. Data © The Monotype Corporation plc/Type Solutions Inc. 1990-1992. All Rights Reserved", "Times New Roman® Trademark of The Monotype Corporation plc registered in the US Pat & TM Off. and elsewhere."));
         localFonts.put("TrebuchetMS", new LocalFont("TrebuchetMS", null, 8, "Copyright (c) 1996 Microsoft Corporation. All rights reserved.", ""));
         localFonts.put("TrebuchetMS-Bold", new LocalFont("TrebuchetMS-Bold", null, 8, "Copyright (c) 1996 Microsoft Corporation. All rights reserved.", ""));
         localFonts.put("Trebuchet-BoldItalic", new LocalFont("Trebuchet-BoldItalic", null, 8, "Copyright (c) 1996 Microsoft Corporation. All rights reserved.", ""));
         localFonts.put("TrebuchetMS-Italic", new LocalFont("TrebuchetMS-Italic", null, 8, "Copyright (c) 1996 Microsoft Corporation. All rights reserved.", ""));
         localFonts.put("Verdana", new LocalFont("Verdana", null, 0, "Typeface and data ? 1996 Microsoft Corporation. All Rights Reserved", "Verdana is a trademark of Microsoft Corporation"));
         localFonts.put("Verdana-Bold", new LocalFont("Verdana-Bold", null, 0, "Typeface and data ? 1996 Microsoft Corporation. All Rights Reserved", "Verdana is a trademark of Microsoft Corporation"));
         localFonts.put("Verdana-Italic", new LocalFont("Verdana-Italic", null, 0, "Typeface and data ? 1996 Microsoft Corporation. All Rights Reserved", "Verdana is a trademark of Microsoft Corporation"));
         localFonts.put("Verdana-BoldItalic", new LocalFont("Verdana-BoldItalic", null, 0, "Typeface and data ? 1996 Microsoft Corporation. All Rights Reserved", "Verdana is a trademark of Microsoft Corporation"));
         localFonts.put("Webdings", new LocalFont("Webdings", null, 0, "Copyright (c) 1997 Microsoft Corporation. All rights reserved.", "Webdings is a trademark of Microsoft Corporation."));
         localFonts.put("Wingdings-Regular", new LocalFont("Wingdings-Regular", null, 0, "Copyright ? 1992-1995 Microsoft Corp. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "The Windows logo is a trademark of Microsoft Corporation."));
         localFonts.put("Wingdings2", new LocalFont("Wingdings2", null, 8, "Wingdings 2 designed by Bigelow & Holmes Inc. for Microsoft Corporation. Copyright ? 1992 Microsoft Corporation. Pat. Pend. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Wingdings is a trademark of Microsoft Corporation."));
         localFonts.put("Wingdings3", new LocalFont("Wingdings3", null, 8, "Wingdings 3 designed by Bigelow & Holmes Inc. for Microsoft Corporation. Copyright ? 1992 Microsoft Corporation. Pat. pend. All Rights Reserved. ? 1990-1991 Type Solutions, Inc. All Rights Reserved.", "Wingdings is a trademark of Microsoft Corporation."));
     }*/
}
