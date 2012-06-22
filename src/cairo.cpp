/**
 * @module builtin/cairo
 * 
 * ## Synopsis
 * 
 * Interface to libcairo graphics library.
 * 
 * http://www.cairographics.org/manual/index.html
 * 
 * Not all functions and constants are available in all SilkJS builds.  Some versions of Ubuntu (Lucid, for one) may not install a new enough version of libcairo to support newer methods and constants provided by cairo.
 * 
 */
#include "SilkJS.h"
#include <stdint.h>
#include <cairo/cairo.h>

////////////////////////// MISC

/**
 * @function cairo.status_to_string
 * 
 * ### Synopsis
 * 
 * var message = cairo.status_to_string(status);
 * 
 * Provides a human-readable description of a cairo.STATUS_* code.
 * 
 * @param {int} status - one of the cairo.STATUS_* codes, as returned by a function like cairo.context_status().
 * @return {string} message - description of the error/status code.
 */
static JSVAL status_to_string(JSARGS args) {
    return String::New(cairo_status_to_string((cairo_status_t)args[0]->IntegerValue()));
}

////////////////////////// SURFACE

/**
 * @function cairo.surface_create_similar
 * 
 * ### Synopsis
 * 
 * var newSurface = cairo.surface_create_similar(handle, format, width, height);
 * 
 * Create a new surface that is as compatible as possible with an existing surface. 
 * 
 * For example the new surface will have the same fallback resolution and font options as other. Generally, the new surface will also use the same backend as other, unless that is not possible for some reason. The type of the returned surface may be examined with cairo.surface_get_type().
 * 
 * Initially the surface contents are all 0 (transparent if contents have transparency, black otherwise.)
 * 
 * Use cairo.surface_create_similar_image() if you need an image surface which can be painted quickly to the target surface.
 *
 * The caller owns the returned surface and should call cairo.surface_destroy() when done with it.
 * 
 * The format parameter is one of the following:
 * 
 * + cairo.CONTENT_COLOR - the surface will hold color content only.
 * + cairo.CONTENT_ALPHA - the surface will hold alpha content only.
 * + cairo.CONTENT_COLOR_ALPHA - the surface will hold color and alpha content.
 *
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {int} format - format of pixels in the surface to create.
 * @param {int} width - width of the surface, in pixels
 * @param {int} height - height of the surface, in pixels.
 * @return {object} newSurface - opaque handle to the newly created surface.
 */
static JSVAL surface_create_similar(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    int format = args[1]->IntegerValue();
    int width = args[2]->IntegerValue();
    int height = args[3]->IntegerValue();
    return External::New(cairo_surface_create_similar(surface, (cairo_content_t)format, width, height));
}

/**
 * @function cairo.surface_reference
 * 
 * ### Synopsis
 * 
 * var surface = cairo.surface_reference(surface);
 * 
 * Increases the reference count on surface by one. This prevents surface from being destroyed until a matching call to cairo.surface_destroy() is made.
 * 
 * The number of references to a cairo surface can be get using cairo.surface_get_reference_count().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} surface - the referenced cairo surface.
 */
static JSVAL surface_reference(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return External::New(cairo_surface_reference(surface));
}

/**
 * @function cairo.surface_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.surface_status(surface);
 * 
 * Checks whether an error has previously occurred for this surface.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} status - one of cairo.STATUS_SUCCESS, cairo.STATUS_NULL_POINTER, cairo.STATUS_NO_MEMORY, cairo.STATUS_READ_ERROR, cairo.STATUS_INVALID_CONTENT, cairo.STATUS_INVALID_FORMAT, or cairo.STATUS_INVALID_VISUAL.
 */
static JSVAL surface_status(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_surface_status(surface));
}

/**
 * @function cairo.surface_destroy
 * 
 * ### Synopsis
 * 
 * cairo.surface_destroy(surface);
 * 
 * Decreases the reference count on surface by one. If the result is zero, then surface and all associated resources are freed. 
 * 
 * See cairo.surface_reference().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_destroy(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_destroy(surface);
    return Undefined();
}

/**
 * @function cairo.surface_finish
 * 
 * ### Synopsis
 * 
 * cairo.surface_finish(surface);
 * 
 * This function finishes the surface and drops all references to external resources. 
 * 
 * For example, for the Xlib backend it means that cairo will no longer access the drawable, which can be freed. 
 * 
 * After calling cairo.surface_finish() the only valid operations on a surface are getting and setting user, referencing and destroying, and flushing and finishing it. Further drawing to the surface will not affect the surface but will instead trigger a cairo.STATUS_SURFACE_FINISHED error.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_finish(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_finish(surface);
    return Undefined();
}

/**
 * @function cairo.surface_flush
 * 
 * ### Synopsis
 * 
 * cairo.surface_flush(surface);
 * 
 * Do any pending drawing for the surface and also restore any temporary modifications cairo has made to the surface's state. 
 * 
 * This function must be called before switching from drawing on the surface with cairo to drawing on it directly with native APIs. 
 * 
 * If the surface doesn't support direct access, then this function does nothing.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_flush(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_flush(surface);
    return Undefined();
}

/**
 * @function cairo.surface_get_device
 * 
 * ### Synopsis
 * 
 * var device = cairo.surface_get_device(surface);
 * 
 * This function returns the device for a surface.
 * 
 * See device methods.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} device - opaque handle to the device for the surface, or null if the surface does not have an associated device.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL surface_get_device(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_device_t *device = cairo_surface_get_device(surface);
    if (device == NULL) {
        return Null();
    }
    return External::New(device);
}
#endif

/**
 * @function cairo.surface_get_font_options
 * 
 * ### Synopsis
 * 
 * var options = cairo.surface_get_font_options(surface);
 * 
 * Retrieves the default font rendering options for the surface. 
 * 
 * This allows display surfaces to report the correct subpixel order for rendering on them, print surfaces to disable hinting of metrics and so forth. 
 * 
 * The result can then be used with cairo.scaled_font_create().
 * 
 * NOTE: the resulting options object should be freed with cairo.font_options_destroy().
 * 
 * The options object returned is an opaque structure holding all options that are used when rendering fonts.
 * 
 * Individual features of a cairo font_options object can be set or accessed using functions named cairo.font_options_set_feature_name() and cairo.font_options_get_feature_name(), like cairo.font_options_set_antialias() and cairo.font_options_get_antialias().
 * 
 * New features may be added to a cairo font_options object in the future. For this reason, cairo.font_options_copy(), cairo.font_options_equal(), cairo.font_options_merge(), and cairo.font_options_hash() should be used to copy, check for equality, merge, or compute a hash value of cairo font_options objects. 
 * 
 * See font_options methods.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} options - opaque handle to the font options for the surface, or null if the surface does not have an associated device.
 */
static JSVAL surface_get_font_options(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_font_options_t *options = cairo_font_options_create();
    cairo_surface_get_font_options(surface, options);
    return External::New(options);
}

/**
 * @function cairo.surface_get_content
 * 
 * ### Synopsis
 * 
 * var content = cairo.surface_get_content(surface);
 * 
 * This function returns the content type of surface which indicates whether the surface contains color and/or alpha information.
 * 
 * The content type returned is one of:
 * 
 * + cairo.CONTENT_COLOR
 * + cairo.CONTENT_ALPHA
 * + cairo.CONTENT_COLOR_ALPHA
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} content - type content type of surface.
 */
static JSVAL surface_get_content(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_surface_get_content(surface));
}

/**
 * @function cairo.surface_mark_dirty
 * 
 * ### Synopsis
 * 
 * cairo.surface_mark_dirty(surface);
 * 
 * Tells cairo that drawing has been done to surface using means other than cairo, and that cairo should reread any cached areas. 
 * 
 * Note that you must call cairo.surface_flush() before doing such drawing.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_mark_dirty(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_mark_dirty(surface);
    return Undefined();
}

/**
 * @function cairo.surface_mark_dirty_rectangle
 * 
 * ### Synopsis
 * 
 * cairo.surface_mark_dirty(surface, x,y, w,h);
 * 
 * Like cairo.surface_mark_dirty(), but drawing has been done only to the specified rectangle, so that cairo can retain cached contents for other parts of the surface.
 * 
 * Any cached clip set on the surface will be reset by this function, to make sure that future cairo calls have the clip set that they expect.
 * 
 * Note that you must call cairo.surface_flush() before doing such drawing.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {int} x - upper left x coordinate of dirty rectangle
 * @param {int} y - upper left y coordinate of dirty rectangle
 * @param {int} w - width of dirty rectangle
 * @param {int} h - height of dirty rectangle
 */
static JSVAL surface_mark_dirty_rectangle(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int w = args[3]->IntegerValue();
    int h = args[4]->IntegerValue();
    cairo_surface_mark_dirty_rectangle(surface, x,y, w,h);
    return Undefined();
}

/**
 * @function cairo.surface_set_device_offset
 * 
 * ### Synopsis
 * 
 * cairo.surface_set_device_offset(surface, x_offset, y_offset);
 * 
 * Sets an offset that is added to the device coordinates determined by the CTM when drawing to surface. 
 * 
 * One use case for this function is when we want to create a cairo surface that redirects drawing for a portion of an on-screen surface to an offscreen surface in a way that is completely invisible to the user of the cairo API. Setting a transformation via cairo.translate() isn't sufficient to do this, since functions like cairo.device_to_user() will expose the hidden offset.
 * 
 * Note that the offset affects drawing to the surface as well as using the surface in a source pattern.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {number} x_offset - the offset in the X direction, in device units
 * @param {number} y_offset - the offset in the Y direction, in device units
 */
static JSVAL surface_set_device_offset(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    double dx = args[1]->NumberValue();
    double dy = args[2]->NumberValue();
    cairo_surface_set_device_offset(surface, dx, dy);
    return Undefined();
}

/**
 * @function cairo.surface_get_device_offset
 * 
 * ### Synopsis
 * 
 * var offsets = cairo.surface_get_device_offset(surface);
 * 
 * This function returns the previous device offset set by cairo.surface_set_device_offset().
 * 
 * The object returned is of the form:
 * 
 * + {number} x_offset: the offset in the X direction, in device units
 * + {number} y_offset: the offset in the Y direction, in device units
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} offsets - offsets.x,offsets.y are floating point x_offset,y_offset values.
 */
static JSVAL surface_get_device_offset(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    double dx;
    double dy;
    cairo_surface_get_device_offset(surface, &dx, &dy);
    JSOBJ o = Object::New();
    o->Set(String::New("x_offset"), Number::New(dx));
    o->Set(String::New("y_offset"), Number::New(dy));
    return o;
}

/**
 * @function cairo.surface_set_fallback_resolution
 * 
 * ### Synopsis
 * 
 * cairo.surface_set_fallback_resolution(surface, x_pixels_per_inch, y_pixels_per_inch);
 * 
 * Set the horizontal and vertical resolution for image fallbacks.
 * 
 * When certain operations aren't supported natively by a backend, cairo will fallback by rendering operations to an image and then overlaying that image onto the output. For backends that are natively vector-oriented, this function can be used to set the resolution used for these image fallbacks, (larger values will result in more detailed images, but also larger file sizes).
 * 
 * Some examples of natively vector-oriented backends are the ps, pdf, and svg backends.
 * 
 * For backends that are natively raster-oriented, image fallbacks are still possible, but they are always performed at the native device resolution. So this function has no effect on those backends.
 * 
 * Note: The fallback resolution only takes effect at the time of completing a page (with cairo.show_page() or cairo.copy_page()) so there is currently no way to have more than one fallback resolution in effect on a single page.
 * 
 * The default fallback resoultion is 300 pixels per inch in both dimensions.
 *  
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {number} x_pixels_per_inch - horizontal setting for pixels per inch.
 * @param {number} y_pixels_per_inch - vertical setting for pixels per inch.
 */
static JSVAL surface_set_fallback_resolution(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    double dx = args[1]->NumberValue();
    double dy = args[2]->NumberValue();
    cairo_surface_set_fallback_resolution(surface, dx, dy);
    return Undefined();
}

/**
 * @function cairo.surface_get_fallback_resolution
 * 
 * ### Synopsis
 * 
 * var resolution = cairo.surface_get_fallback_resolution(surface);
 * 
 * This function returns the previous device offset set by cairo.surface_set_fallback_resolution().
 * 
 * The object returned is of the form:
 * 
 * + {number} x_pixels_per_inch: the offset in the X direction, in device units
 * + {number} y_pixels_per_inch: the offset in the Y direction, in device units
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} resolution - resolution.x,resolution.y are floating point pixels_per_inch values.
 */
static JSVAL surface_get_fallback_resolution(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    double dx;
    double dy;
    cairo_surface_get_fallback_resolution(surface, &dx, &dy);
    JSOBJ o = Object::New();
    o->Set(String::New("x_offset"), Number::New(dx));
    o->Set(String::New("y_offset"), Number::New(dy));
    return o;
}

/**
 * @function cairo.surface_get_type
 * 
 * ### Synopsis
 * 
 * var type = cairo.surface_get_type(surface);
 * 
 * This function returns the type of the backend used to create a surface.
 * 
 * The returned type will be one of the following:
 * 
 * + cairo.SURFACE_TYPE_IMAGE - The surface is of type image
 * + cairo.SURFACE_TYPE_PDF - The surface is of type pdf
 * + cairo.SURFACE_TYPE_PS - The surface is of type ps
 * + cairo.SURFACE_TYPE_XLIB - The surface is of type xlib
 * + cairo.SURFACE_TYPE_XCB - The surface is of type xcb
 * + cairo.SURFACE_TYPE_GLITZ - The surface is of type glitz
 * + cairo.SURFACE_TYPE_QUARTZ - The surface is of type quartz
 * + cairo.SURFACE_TYPE_WIN32 - The surface is of type win32
 * + cairo.SURFACE_TYPE_BEOS - The surface is of type beos
 * + cairo.SURFACE_TYPE_DIRECTFB - The surface is of type directfb
 * + cairo.SURFACE_TYPE_SVG - The surface is of type svg
 * + cairo.SURFACE_TYPE_OS2 - The surface is of type os2
 * + cairo.SURFACE_TYPE_WIN32_PRINTING - The surface is a win32 printing surface
 * + cairo.SURFACE_TYPE_QUARTZ_IMAGE - The surface is of type quartz_image
 * + cairo.SURFACE_TYPE_SCRIPT - The surface is of type script, since 1.10
 * + cairo.SURFACE_TYPE_QT - The surface is of type Qt, since 1.10
 * + cairo.SURFACE_TYPE_RECORDING - The surface is of type recording, since 1.10
 * + cairo.SURFACE_TYPE_VG - The surface is a OpenVG surface, since 1.10
 * + cairo.SURFACE_TYPE_GL - The surface is of type OpenGL, since 1.10
 * + cairo.SURFACE_TYPE_DRM - The surface is of type Direct Render Manager, since 1.10
 * + cairo.SURFACE_TYPE_TEE - The surface is of type 'tee' (a multiplexing surface), since 1.10
 * + cairo.SURFACE_TYPE_XML - The surface is of type XML (for debugging), since 1.10
 * + cairo.SURFACE_TYPE_SKIA - The surface is of type Skia, since 1.10
 * + cairo.SURFACE_TYPE_SUBSURFACE - The surface is a subsurface created with cairo_surface_create_for_rectangle(), since 1.10
 * + cairo.SURFACE_TYPE_COGL - This surface is of type Cogl, since 1.12
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} type - one of the types above.
 */
static JSVAL surface_get_type(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_surface_get_type(surface));
}

/**
 * @function cairo.surface_get_reference_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.surface_get_reference_count(surface);
 * 
 * Returns the current reference count of surface.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} count - reference count of surface.  If the object is a nil object, 0 will be returned.
 */
static JSVAL surface_get_reference_count(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_surface_get_reference_count(surface));
}

/**
 * @function cairo.surface_copy_page
 * 
 * ### Synopsis
 * 
 * cairo.surface_copy_page(surface);
 * 
 * Emits the current page for backends that support multiple pages, but doesn't clear it, so that the contents of the current page will be retained for the next page. 
 * 
 * Use cairo.surface_show_page() if you want to get an empty page after the emission.
 * 
 * There is a convenience function for this that takes a Context, namely cairo.copy_page().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_copy_page(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_copy_page(surface);
    return Undefined();
}

/**
 * @function cairo.surface_show_page
 * 
 * ### Synopsis
 * 
 * cairo.surface_show_page(surface);
 * 
 * Emits and clears the current page for backends that support multiple pages. Use cairo.surface_copy_page() if you don't want to clear the page.
 * 
 * There is a convenience function for this that takes a Context, namely cairo.show_page().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_show_page(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_show_page(surface);
    return Undefined();
}

/**
 * @function cairo.surface_has_show_text_glyphs
 * 
 * ### Synopsis
 * 
 * var status = cairo.surface_has_show_text_glyphs(surface);
 * 
 * Returns whether the surface supports sophisticated cairo.show_text_glyphs() operations. That is, whether it actually uses the provided text and cluster data to a cairo.show_text_glyphs() call.
 * 
 * Note: Even if this function returns false, a cairo.show_text_glyphs() operation targeted at surface will still succeed. It just will act like a cairo.show_glyphs() operation. 
 * 
 * Users can use this function to avoid computing UTF-8 text and cluster mapping if the target surface does not use it.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {boolean} status - true if the surface supports show_text_glyphs().
 */
static JSVAL surface_has_show_text_glyphs(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return cairo_surface_has_show_text_glyphs(surface) ? True() : False();
}


/**
 * @function cairo.image_surface_create
 * 
 * ### Synopsis
 * 
 * var surface = cairo.image_surface_create(format, width, height);
 * 
 * Creates an image surface of the specified format and dimensions. Initially the surface contents are all 0. (Specifically, within each pixel, each color or alpha channel belonging to format will be 0. The contents of bits within a pixel, but not belonging to the given format are undefined).
 * 
 * This function always returns a valid handle, but it will return a handle to a "nil" surface if an error such as out of memory occurs. 
 * 
 * You can use cairo.surface_status() to check for this.
 * 
 * The format parameter is one of the following:
 * 
 * + cairo.CONTENT_COLOR - the surface will hold color content only.
 * + cairo.CONTENT_ALPHA - the surface will hold alpha content only.
 * + cairo.CONTENT_COLOR_ALPHA - the surface will hold color and alpha content.
 *
 * @param {int} format - format of pixels in the surface to create.
 * @param {int} width - width of the surface, in pixels
 * @param {int} height - height of the surface, in pixels.
 * @return {object} surface - opaque handle to the newly created surface. The caller owns the surface and should call cairo.surface_destroy() when done with it. 
 */
static JSVAL image_surface_create(JSARGS args) {
    int format = args[0]->IntegerValue();
    int width = args[1]->IntegerValue();
    int height = args[2]->IntegerValue();
    return External::New(cairo_image_surface_create((cairo_format_t)format, width, height));
}

/**
 * @functino cairo.image_surface_get_format
 * 
 * ### Synopsis
 * 
 * var format = cairo.image_surface_get_format(surface);
 * 
 * Get the format of the surface.
 * 
 * The format returned will be one of the following:
 * 
 * + cairo.FORMAT_ARGB32
 * + cairo.FORMAT_RGB24
 * + cairo.FORMAT_A8
 * + cairo.FORMAT_A1
 * + cairo.FORMAT_RGB16_565
 * + cairo.FORMAT_RGB30
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} format - one of the above values.
 */
static JSVAL image_surface_get_format(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_image_surface_get_format(surface));
}

/**
 * @functino cairo.image_surface_get_width
 * 
 * ### Synopsis
 * 
 * var width = cairo.image_surface_get_width(surface);
 * 
 * Get the width of the surface.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} width - width of the surface in pixels.
 */
static JSVAL image_surface_get_width(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_image_surface_get_width(surface));
}

/**
 * @functino cairo.image_surface_get_height
 * 
 * ### Synopsis
 * 
 * var height = cairo.image_surface_get_height(surface);
 * 
 * Get the height of the surface.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} height - height of the surface in pixels.
 */
static JSVAL image_surface_get_height(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_image_surface_get_height(surface));
}

/**
 * @function cairo.image_surface_get_data
 * 
 * ### Synopsis
 * 
 * var imageData = cairo.image_surface_get_data(surface, sx, sy, width, height);
 * 
 * Gets a canvas style ImageData object.
 * 
 * The imageData object returned is of the following form:
 * 
 * + {int} width - width of the image data pixel array in device pixels.
 * + {int} height - height of the image data pixel array in device pixels.
 * + {array} data - pixel data as a one dimensional array in RGBA order, as integers in the range 0..255.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} imageData - object of the above form.
 */
static JSVAL image_surface_get_data(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    int sx = args[1]->IntegerValue();
    int sy = args[2]->IntegerValue();
    int width = args[3]->IntegerValue();
    int height = args[4]->IntegerValue();
    int length = width * height;
    int stride = cairo_image_surface_get_stride(surface);
    int cWidth = cairo_image_surface_get_width(surface);
    int cHeight = cairo_image_surface_get_height(surface);
    if (sx < 0) {
        width += sx;
        sx = 0;
    }
    if (sy < 0) {
        height += sy;
        sy = 0;
    }
    if (sx + width > cWidth) {
        width = cWidth - sx;
    }
    if (sy + height > cHeight) {
        height = cHeight - sy;
    }
    cairo_surface_flush(surface);
    uint8_t *src = cairo_image_surface_get_data(surface);
    Handle<Array>bytes = Array::New(length*4);
    int ndx = 0;
    for (int y = 0; y<height; y++) {
        uint32_t *row = (uint32_t *)(src + stride * (y + sy));
        for (int x=0; x<width; x++) {
//            int bx = x*4;
            uint32_t *pixel = row + x + sx;
            uint8_t a = *pixel >> 24;
            uint8_t r = *pixel >> 16;
            uint8_t g = *pixel >> 8;
            uint8_t b = *pixel;
            float alpha = (float)a / 255;
            bytes->Set(ndx++, Integer::New((int)((float) r / alpha)));
            bytes->Set(ndx++, Integer::New((int)((float) g / alpha)));
            bytes->Set(ndx++, Integer::New((int)((float) b / alpha)));
            bytes->Set(ndx++, Integer::New(a));
        }
    }
    JSOBJ o = Object::New();
    o->Set(String::New("data"), bytes);
    o->Set(String::New("width"), Integer::New(width));
    o->Set(String::New("height"), Integer::New(height));
    return o;
}

/**
 * @function cairo.surface_blur
 * 
 * ### Synopsis
 * 
 * cairo.surface_blur(surface, radius);
 * 
 * Blur the given surface with the given radius.
 * 
 * ### Note
 * 
 * This is a helper function to implement Canvas class; it is not a part of Cairo.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {int} radius - radius to blur
 */
static JSVAL surface_blur(JSARGS args) {
    // see implementation at https://github.com/LearnBoost/node-canvas/blob/master/src/CanvasRenderingContext2d.cc
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    int radius = args[1]->IntegerValue();

    // Steve Hanov, 2009
    // Released into the public domain.
    --radius;
    // get width, height
    int width = cairo_image_surface_get_width(surface);
    int height = cairo_image_surface_get_height(surface);
    unsigned* precalc =
        (unsigned*) malloc(width * height * sizeof (unsigned));
    unsigned char* src = cairo_image_surface_get_data(surface);
    double mul = 1.f / ((radius * 2)*(radius * 2));
    int channel;

    // The number of times to perform the averaging. According to wikipedia,
    // three iterations is good enough to pass for a gaussian.
    const int MAX_ITERATIONS = 3;
    int iteration;

    for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        for (channel = 0; channel < 4; channel++) {
            int x, y;

            // precomputation step.
            unsigned char* pix = src;
            unsigned* pre = precalc;

            pix += channel;
            for (y = 0; y < height; y++) {
                for (x = 0; x < width; x++) {
                    int tot = pix[0];
                    if (x > 0) tot += pre[-1];
                    if (y > 0) tot += pre[-width];
                    if (x > 0 && y > 0) tot -= pre[-width - 1];
                    *pre++ = tot;
                    pix += 4;
                }
            }

            // blur step.
            pix = src + (int) radius * width * 4 + (int) radius * 4 + channel;
            for (y = radius; y < height - radius; y++) {
                for (x = radius; x < width - radius; x++) {
                    int l = x < radius ? 0 : x - radius;
                    int t = y < radius ? 0 : y - radius;
                    int r = x + radius >= width ? width - 1 : x + radius;
                    int b = y + radius >= height ? height - 1 : y + radius;
                    int tot = precalc[r + b * width] + precalc[l + t * width] -
                        precalc[l + b * width] - precalc[r + t * width];
                    *pix = (unsigned char) (tot * mul);
                    pix += 4;
                }
                pix += (int) radius * 2 * 4;
            }
        }
    }

    free(precalc);
    return Undefined();
}

////////////////////// CONTEXTS

/**
 * @function cairo.context_create
 * 
 * ### Synopsis
 * 
 * var context = cairo.context_create(surface);
 * 
 * Creates a new context with all graphics state parameters set to default values and with surface as a target surface. The target surface should be constructed with a backend-specific function such as cairo.image_surface_create() (or any other cairo.backend_surface_create() variant).
 * 
 * This function references surface, so you can immediately call cairo.surface_destroy() on it if you don't need to maintain a separate reference to it.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} context - opaque handle to a cairo context.
 */
static JSVAL context_create(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return External::New(cairo_create(surface));
}

/**
 * @function cairo.context_reference
 * 
 * ### Synopsis
 * 
 * var context = cairo.context_reference(context);
 * 
 * Increases the reference count on context by one. This prevents context from being destroyed until a matching call to cairo.context_destroy() is made.
 * 
 * The number of references to a context can be get using cairo.context_get_reference_count().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} context - opaque handle to the referenced cairo context.
 */
static JSVAL context_reference(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_reference(context));
}

/**
 * @function cairo.context_get_reference_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.context_get_reference_count(context);
 * 
 * Returns the current reference count of context.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} count - reference count of the context.
 */
static JSVAL context_get_reference_count(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_reference_count(context));
}

/**
 * @function cairo.context_destroy
 * 
 * ### Synopsis
 * 
 * cairo.context_destroy(context);
 * 
 * Decreases the reference count on context by one. If the result is zero, then context and all associated resources are freed. 
 * 
 * See cairo.context_reference().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_destroy(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_destroy(context);
    return Undefined();
}

/**
 * @function cairo.context_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.context_status(context);
 * 
 * Returns the previous error status code for this context.
 * 
 * The status returned is one of the following:
 *  
 * + cairo.STATUS_SUCCESS - no error has occurred
 * + cairo.STATUS_NO_MEMORY - out of memory
 * + cairo.STATUS_INVALID_RESTORE - cairo_restore() called without matching cairo_save()
 * + cairo.STATUS_INVALID_POP_GROUP - no saved group to pop, i.e. cairo_pop_group() without matching cairo_push_group()
 * + cairo.STATUS_NO_CURRENT_POINT - no current point defined
 * + cairo.STATUS_INVALID_MATRIX - invalid matrix (not invertible)
 * + cairo.STATUS_INVALID_STATUS - invalid value for an input + cairo.STATUS_t
 * + cairo.STATUS_NULL_POINTER - NULL pointer
 * + cairo.STATUS_INVALID_STRING - input string not valid UTF-8
 * + cairo.STATUS_INVALID_PATH_DATA - input path data not valid
 * + cairo.STATUS_READ_ERROR - error while reading from input stream
 * + cairo.STATUS_WRITE_ERROR - error while writing to output stream
 * + cairo.STATUS_SURFACE_FINISHED - target surface has been finished
 * + cairo.STATUS_SURFACE_TYPE_MISMATCH - the surface type is not appropriate for the operation
 * + cairo.STATUS_PATTERN_TYPE_MISMATCH - the pattern type is not appropriate for the operation
 * + cairo.STATUS_INVALID_CONTENT - invalid value for an input cairo_content_t
 * + cairo.STATUS_INVALID_FORMAT - invalid value for an input cairo_format_t
 * + cairo.STATUS_INVALID_VISUAL - invalid value for an input Visual*
 * + cairo.STATUS_FILE_NOT_FOUND - file not found
 * + cairo.STATUS_INVALID_DASH - invalid value for a dash setting
 * + cairo.STATUS_INVALID_DSC_COMMENT - invalid value for a DSC comment (Since 1.2)
 * + cairo.STATUS_INVALID_INDEX - invalid index passed to getter (Since 1.4)
 * + cairo.STATUS_CLIP_NOT_REPRESENTABLE - clip region not representable in desired format (Since 1.4)
 * + cairo.STATUS_TEMP_FILE_ERROR - error creating or writing to a temporary file (Since 1.6)
 * + cairo.STATUS_INVALID_STRIDE - invalid value for stride (Since 1.6)
 * + cairo.STATUS_FONT_TYPE_MISMATCH - the font type is not appropriate for the operation (Since 1.8)
 * + cairo.STATUS_USER_FONT_IMMUTABLE - the user-font is immutable (Since 1.8)
 * + cairo.STATUS_USER_FONT_ERROR - error occurred in a user-font callback function (Since 1.8)
 * + cairo.STATUS_NEGATIVE_COUNT - negative number used where it is not allowed (Since 1.8)
 * + cairo.STATUS_INVALID_CLUSTERS - input clusters do not represent the accompanying text and glyph array (Since 1.8)
 * + cairo.STATUS_INVALID_SLANT - invalid value for an input cairo_font_slant_t (Since 1.8)
 * + cairo.STATUS_INVALID_WEIGHT - invalid value for an input cairo_font_weight_t (Since 1.8)
 * + cairo.STATUS_INVALID_SIZE - invalid value (typically too big) for the size of the input (surface, pattern, etc.) (Since 1.10)
 * + cairo.STATUS_USER_FONT_NOT_IMPLEMENTED - user-font method not implemented (Since 1.10)
 * + cairo.STATUS_DEVICE_TYPE_MISMATCH - the device type is not appropriate for the operation (Since 1.10)
 * + cairo.STATUS_DEVICE_ERROR - an operation to the device caused an unspecified error (Since 1.10)
 * + cairo.STATUS_INVALID_MESH_CONSTRUCTION - a mesh pattern construction operation was used outside of a cairo_mesh_pattern_begin_patch()/cairo_mesh_pattern_end_patch() pair (Since 1.12)
 * + cairo.STATUS_DEVICE_FINISHED - target device has been finished (Since 1.12)
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} status - see possible values above.
 */
static JSVAL context_status(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_status(context));
}

/**
 * @function cairo.context_save
 * 
 * ### Synopsis
 * 
 * cairo.context_save(context);
 * 
 * Makes a copy of the current state of context and saves it on an internal stack of saved states for context. 
 * 
 * When cairo.context_restore() is called, context will be restored to the saved state. 
 * 
 * Multiple calls to cairo.context_save() and cairo.context_restore() can be nested; each call to cairo.context_restore() restores the state from the matching paired cairo.context_save().
 * 
 * It isn't necessary to clear all saved states before a context is freed. 
 * 
 * If the reference count of a context drops to zero in response to a call to cairo.context_destroy(), any saved states will be freed along with the context.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_save(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_save(context);
    return Undefined();
}

/**
 * @function cairo.context_restore
 * 
 * ### Synopsis
 * 
 * cairo.context_restore(context);
 * 
 * Restores context to the state saved by a preceding call to cairo.context_save() and removes that state from the stack of saved states.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_restore(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_restore(context);
    return Undefined();
}

/**
 * @function cairo.context_get_target
 * 
 * ### Synopsis
 * 
 * var surface = cairo.context_get_target(context);
 * 
 * Gets the target surface for the cairo context as passed to cairo.context_create().
 * 
 * This function will always return a valid pointer, but the result can be a "nil" surface if cr is already in an error state, (ie. cairo.context_status() != cairo.STATUS_SUCCESS).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} surface - opaque handle to the cairo surface.
 */
static JSVAL context_get_target(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_get_target(context));
}

/**
 * @function cairo.context_push_group
 * 
 * ### Synopsis
 * 
 * cairo.context_push_group(context);
 * 
 * Temporarily redirects drawing to an intermediate surface known as a group. 
 * 
 * The redirection lasts until the group is completed by a call to cairo.context_pop_group() or cairo.context_pop_group_to_source(). 
 * 
 * These calls provide the result of any drawing to the group as a pattern, (either as an explicit object, or set as the source pattern).
 * 
 * This group functionality can be convenient for performing intermediate compositing. One common use of a group is to render objects as opaque within the group, (so that they occlude each other), and then blend the result with translucence onto the destination.
 * 
 * Groups can be nested arbitrarily deep by making balanced calls to cairo.context_push_group()/cairo.context_pop_group(). Each call pushes/pops the new target group onto/from a stack.
 * 
 * The cairo.context_push_group() function calls cairo.context_save() so that any changes to the graphics state will not be visible outside the group, (the cairo.content_pop_group() functions call cairo.context__restore()).
 * 
 * By default the intermediate group will have a content type of cairo.CONTENT_COLOR_ALPHA. Other content types can be chosen for the group by using cairo.context_push_group_with_content() instead.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * 
 * ### Example
 * 
 * As an example, here is how one might fill and stroke a path with translucence, but without any portion of the fill being visible under the stroke:
 * 
 * ```
 * cairo.context_push_group (context);
 * cairo.context_set_source (context, fill_pattern);
 * cairo.context_fill_preserve (context);
 * cairo.context_set_source (context, stroke_pattern);
 * cairo.context_stroke (context);
 * cairo.context_pop_group_to_source (context);
 * cairo.context_paint_with_alpha (context, alpha);
 * ```
 */
static JSVAL context_push_group(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_push_group(context);
    return Undefined();
}

/**
 * @function cairo.context_push_group_with_content
 * 
 * ### Synopsis
 * 
 * cairo.context_push_group_with_content(context, content);
 * 
 * Temporarily redirects drawing to an intermediate surface known as a group. The redirection lasts until the group is completed by a call to cairo.context_pop_group() or cairo.context_pop_group_to_source(). These calls provide the result of any drawing to the group as a pattern, (either as an explicit object, or set as the source pattern).
 * 
 * The group will have a content type of content. The ability to control this content type is the only distinction between this function and cairo.content_push_group() which you should see for a more detailed description of group rendering.
 * 
 * The content parameter is one of the following value:
 * 
 * + cairo.CONTENT_COLOR - the surface will hold color content only.
 * + cairo.CONTENT_ALPHA - the surface will hold alpha content only.
 * + cairo.CONTENT_COLOR_ALPHA - the surface will hold color and alpha content.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_push_group_with_content(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_push_group_with_content(context, (cairo_content_t)args[0]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.context_pop_group
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.context_pop_group(context);
 * 
 * Terminates the redirection begun by a call to cairo.context_push_group() or cairo.context_push_group_with_content() and returns a new pattern containing the results of all drawing operations performed to the group.
 * 
 * The cairo.context_pop_group() function calls cairo.context_restore(), (balancing a call to cairo.context_save() by the cairo.context_push_group() function), so that any changes to the graphics state will not be visible outside the group.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL context_pop_group(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_pop_group(context));
}

/**
 * @function cairo.context_pop_group_to_source
 * 
 * ### Synopsis
 * 
 * cairo.context_pop_group_to_source(context);
 * 
 * Terminates the redirection begun by a call to cairo.context_push_group() or cairo,context_push_group_with_content() and installs the resulting pattern as the source pattern in the given cairo context.
 * 
 * The behavior of this function is equivalent to the sequence of operations:
 * 
 * ```
 * var group = cairo.context_pop_group (context);
 * cairo.context_set_source (context, group);
 * cairo.pattern_destroy (group);
 * ```
 * but is more convenient as their is no need for a variable to store the short-lived pointer to the pattern.
 * 
 * The cairo.context_pop_group() function calls cairo.context_restore(), (balancing a call to cairo.context_save() by the cairo.context_push_group()function), so that any changes to the graphics state will not be visible outside the group.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_pop_group_to_source(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_pop_group_to_source(context);
    return Undefined();
}

/**
 * @function cairo.context_get_group_target
 * 
 * ### Synopsis
 * 
 * var surface = cairo.context_get_group_target(context);
 * 
 * Gets the current destination surface for the context. This is either the original target surface as passed to cairo.context_create() or the target surface for the current group as started by the most recent call to cairo.context_push_group() or cairo.context_push_group_with_content().
 * 
 * This function will always return a valid pointer, but the result can be a "nil" surface if context is already in an error state, (ie. cairo.context_status() != cairo.STATUS_SUCCESS).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} surface - opaque handle to the cairo surface.
 */
static JSVAL context_get_group_target(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_get_group_target(context));
}

/**
 * @function cairo.context_set_source_rgb
 * 
 * ### Synopsis
 * 
 * cairo.context_set_source_rgb(context, red, green, blue);
 * 
 * Sets the source pattern within context to an opaque color. 
 * 
 * This opaque color will then be used for any subsequent drawing operation until a new source pattern is set.
 * 
 * The color components are floating point numbers in the range 0 to 1. If the values passed in are outside that range, they will be clamped.
 * 
 * The default source pattern is opaque black, (that is, it is equivalent to cairo.context_set_source_rgb(context, 0.0, 0.0, 0.0)).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} red - red component of color
 * @param {number} green - green component of color
 * @param {number} blue - blue component of color
 */
static JSVAL context_set_source_rgb(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_source_rgb(context, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_set_source_rgba
 * 
 * ### Synopsis
 * 
 * cairo.context_set_source_rgba(context, red, green, blue, alpha);
 * 
 * Sets the source pattern within context to a translucent color. 
 * 
 * This color will then be used for any subsequent drawing operation until a new source pattern is set.
 * 
 * The color and alpha components are floating point numbers in the range 0 to 1. If the values passed in are outside that range, they will be clamped.
 * 
 * The default source pattern is opaque black, (that is, it is equivalent to cairo.context_set_source_rgba(context, 0.0, 0.0, 0.0, 1.0)).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} red - red component of color
 * @param {number} green - green component of color
 * @param {number} blue - blue component of color
 * @param {number} alpha - alpha component of color
 */
static JSVAL context_set_source_rgba(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_source_rgba(context, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue(), args[4]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_set_source
 * 
 * ### Synopsis
 * 
 * cairo.context_set_source(context, pattern);
 * 
 * Sets the source pattern within context to pattern. 
 * 
 * This pattern will then be used for any subsequent drawing operation until a new source pattern is set.
 * 
 * Note: The pattern's transformation matrix will be locked to the user space in effect at the time of cairo.context_set_source(). This means that further modifications of the current transformation matrix will not affect the source pattern. See cairo.pattern_set_matrix().
 * 
 * The default source pattern is a solid pattern that is opaque black, (that is, it is equivalent to cairo.context_set_source_rgb(context, 0.0, 0.0, 0.0)).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL context_set_source(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[1]);
    cairo_set_source(context, pattern);
    return Undefined();
}

/**
 * @function cairo.context_set_source_surface
 * 
 * ### Synopsis
 * 
 * cairo.context_set_source_surface(context, surface, x, y);
 * 
 * This is a convenience function for creating a pattern from surface and setting it as the source in context with cairo.context_set_source().
 * 
 * The x and y parameters give the user-space coordinate at which the surface origin should appear. (The surface origin is its upper-left corner before any transformation has been applied.) The x and y parameters are negated and then set as translation values in the pattern matrix.
 * 
 * Other than the initial translation pattern matrix, as described above, all other pattern attributes, (such as its extend mode), are set to the default values as in cairo.pattern_create_for_surface(). The resulting pattern can be queried with cairo.context_get_source() so that these attributes can be modified if desired, (eg. to create a repeating pattern with cairo.pattern_set_extend()).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} surface - opaque handle to a cairo surface..
 * @param {number} x - x coordinate of surface origin.
 * @param {number} y - y coordinate of surface origin.
 */
static JSVAL context_set_source_surface(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[1]);
    double x = args[2]->NumberValue();
    double y = args[3]->NumberValue();
    cairo_set_source_surface(context, surface, x, y);
    return Undefined();
}

/**
 * @function cairo.context_get_source
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.context_get_source(context, pattern);
 * 
 * Gets the current source pattern within context. 
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL context_get_source(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_get_source(context));
}

/**
 * @function cairo.context_set_antialias
 * 
 * ### Synopsis
 * 
 * cairo.context_set_antialias(context, mode);
 * 
 * Set the antialiasing mode of the rasterizer used for drawing shapes. 
 * 
 * This value is a hint, and a particular backend may or may not support a particular value. At the current time, no backend supports cairo.ANTIALIAS_SUBPIXEL when drawing shapes.
 * 
 * Note that this option does not affect text rendering, instead see cairo.context_font_options_set_antialias().
 * 
 * Valid modes are:
 * 
 * + cairo.ANTIALIAS_DEFAULT - use the default antialiasing for the subsystem and target device.
 * + cairo.ANTIALIAS_NONE - use a bilevel alpha mask.
 * + cairo.ANTIALIAS_GRAY - perform single-color antialiasing (using shades of gray for black text on a white background, for example).
 * + cairo.ANTIALIAS_SUBPIXEL - perform antialiasing by taking advantage of the order of subpixel elements on devices such as LCD panels.
 * + cairo_ANTIALIAS_FAST - Hint that the backend should perform some antialiasing but prefer speed over quality, since 1.12.
 * + cairo.ANTIALIAS_GOOD - The backend should balance quality against performance, since 1.12.
 * + CAIRO_ANTIALIAS_BEST - Hint that the backend should render at the highest quality, sacrificing speed if necessary, since 1.12
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} mode - one of the above modes.
 */
static JSVAL context_set_antialias(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_antialias(context, (cairo_antialias_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.context_get_antialias
 * 
 * ### Synopsis
 * 
 * var mode = cairo.context_get_antialias(context);
 * 
 * Gets the current shape antialiasing mode, as set by cairo.context_set_antialias().
 * 
 * The return value is a mode:
 * 
 * Valid modes are:
 * 
 * + cairo.ANTIALIAS_DEFAULT - use the default antialiasing for the subsystem and target device.
 * + cairo.ANTIALIAS_NONE - use a bilevel alpha mask.
 * + cairo.ANTIALIAS_GRAY - perform single-color antialiasing (using shades of gray for black text on a white background, for example).
 * + cairo.ANTIALIAS_SUBPIXEL - perform antialiasing by taking advantage of the order of subpixel elements on devices such as LCD panels.
 * + cairo_ANTIALIAS_FAST - Hint that the backend should perform some antialiasing but prefer speed over quality, since 1.12.
 * + cairo.ANTIALIAS_GOOD - The backend should balance quality against performance, since 1.12.
 * + CAIRO_ANTIALIAS_BEST - Hint that the backend should render at the highest quality, sacrificing speed if necessary, since 1.12
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} mode - see list above.
 */
static JSVAL context_get_antialias(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_antialias(context));
}

/**
 * @function cairo.content_set_dash
 * 
 * ### Synopsis
 * 
 * cairo.context_set_dash(context, dashes, offset);
 * 
 * Sets the dash pattern to be used by cairo.context_stroke(). 
 * 
 * A dash pattern is specified by dashes, an array of positive values. 
 * 
 * Each value provides the length of alternate "on" and "off" portions of the stroke. 
 * 
 * The offset specifies an offset into the pattern at which the stroke begins.
 * 
 * Each "on" segment will have caps applied as if the segment were a separate sub-path. In particular, it is valid to use an "on" length of 0.0 with cairo.LINE_CAP_ROUND or cairo.LINE_CAP_SQUARE in order to distributed dots or squares along a path.
 * 
 * Note: The length values are in user-space units as evaluated at the time of stroking. This is not necessarily the same as the user space at the time of cairo.context_set_dash()..
 * 
 * If the dashes array has length 0, dashing is disabled.
 * 
 * If the dashes array has a single element,  a symmetric pattern is assumed with alternating on and off portions of the size specified by the single value in dashes.
 * 
 * If any value in dashes is negative, or if all values are 0, then context will be put into an error state with a status of cairo.STATUS_INVALID_DASH.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {array} dashes - array of dash information, described above.
 * @param {number} offset - offset into pattern at which the stroke begins.
 */
static JSVAL context_set_dash(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Handle<Array>dashes = Handle<Array>::Cast(args[1]->ToObject());
    double offset = args[2]->NumberValue();
    
    int numDashes = dashes->Length();
    if (numDashes == 0) {
        cairo_set_dash(context, NULL, 0, offset);
    }
    double *dashArray = new double[numDashes];
    for (int i=0; i<numDashes; i++) {
        dashArray[i] = dashes->Get(i)->NumberValue();
    }
    cairo_set_dash(context, dashArray, numDashes, offset);
    delete [] dashArray;
    
    return Undefined();
}

/**
 * @function cairo.context_get_dash_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.context_get_dash_count(context);
 * 
 * Returns the length of the dash array in the context, or 0 if dashing is not in effect.
 * 
 * See also cairo.context_set_dash() and cairo_context.get_dash().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} count - length of dashes array.
 */
static JSVAL context_get_dash_count(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_dash_count(context));
}

/**
 * @function cairo.context_get_dash
 * 
 * ### Synopsis
 * 
 * var dashes = cairo.context_get_dash(context);
 * 
 * Gets the current dash array.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {array} dashes = array of Number values of the dash array for the context.
 */
static JSVAL context_get_dash(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    
    int count = cairo_get_dash_count(context);
    double *dashes = new double[count];
    double offset;
    cairo_get_dash(context, dashes, &offset);
    Handle<Array>a = Array::New(count);
    for (int i=0; i<count; i++) {
        a->Set(i, Number::New(dashes[i]));
    }
    delete [] dashes;
    return a;
}

/**
 * @function cairo.context_set_fill_rule
 * 
 * ### Synopsis
 * 
 * cairo.context_set_fill_rule(context, rule);
 * 
 * Set the current fill rule within the cairo context. The fill rule is used to determine which regions are inside or outside a complex (potentially self-intersecting) path. The current fill rule affects both cairo.context_fill() and cairo.context_clip().
 * 
 * The rule argument may be:
 * 
 * cairo.FILL_RULE_WINDING
 *   If the path crosses the ray from left-to-right, counts +1. If the path crosses the ray from right to left, counts -1. (Left and right are determined from the perspective of looking along the ray from the starting point.) If the total count is non-zero, the point will be filled.
 * cairo.FILL_RULE_EVEN_ODD
 *   Counts the total number of intersections, without regard to the orientation of the contour. If the total number of intersections is odd, the point will be filled.
 * 
 * For both fill rules, whether or not a point is included in the fill is determined by taking a ray from that point to infinity and looking at intersections with the path. The ray can be in any direction, as long as it doesn't pass through the end point of a segment or have a tricky intersection such as intersecting tangent to the path. (Note that filling is not actually implemented in this way. This is just a description of the rule that is applied.)
 * 
 * The default fill rule is cairo.FILL_RULE_WINDING.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} rule - one of the above fill rules.
 */
static JSVAL context_set_fill_rule(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    int rule = args[1]->IntegerValue();
    cairo_set_fill_rule(context, (cairo_fill_rule_t)rule);
    return Undefined();
}

/**
 * @function cairo.context_get_fill_rule
 * 
 * ### Synopsis
 * 
 * var rule = cairo.context_get_fill_rule(context);
 * 
 * Gets the current fill rule, as set by cairo.context_set_fill_rule().
 * 
 * See cairo.context_set_fill_rule() for description of the fill rule values.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} rule - fill rule
 */
static JSVAL context_get_fill_rule(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_fill_rule(context));
}

/**
 * @function cairo.context_set_line_cap
 * 
 * ### Synopsis
 * 
 * cairo.context_set_line_cap(context, line_cap);
 * 
 * Sets the current line cap style within the cairo context. 
 * 
 * As with the other stroke parameters, the current line cap style is examined by cairo.context_stroke(), cairo.context_stroke_extents(), and cairo.context_stroke_to_path(), but does not have any effect during path construction.
 * 
 * The line_cap values may be:
 * 
 * cairo.LINE_CAP_BUTT
 *   start(stop) the line exactly at the start(end) point
 * cairo.LINE_CAP_ROUND
 *   use a round ending, the center of the circle is the end point
 * cairo.LINE_CAP_SQUARE
 *   use squared ending, the center of the square is the end point
 * 
 * The default line cap style is cairo.LINE_CAP_BUTT.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} line_cap - one of the above line_cap values.
 */
static JSVAL context_set_line_cap(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    int line_cap = args[1]->IntegerValue();
    cairo_set_line_cap(context, (cairo_line_cap_t)line_cap);
    return Undefined();
}

/**
 * @function cairo.context_get_line_cap
 * 
 * ### Synopsis
 * 
 * var line_cap = cairo.context_get_line_cap(context);
 * 
 * Gets the current line cap style, as set by cairo.context_set_line_cap().
 * 
 * See cairo.context_set_line_cap() for description of the line_cap values.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} line_cap - line cap style.
 */
static JSVAL context_get_line_cap(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_line_cap(context));
}

/**
 * @function cairo.context_set_line_join
 * 
 * ### Synopsis
 * 
 * cairo.context_set_line_join(context, line_join);
 * 
 * Sets the current line join style within the cairo context. 
 * 
 * As with the other stroke parameters, the current line join style is examined by cairo.context_stroke(), cairo.context_stroke_extents(), and cairo.context_stroke_to_path(), but does not have any effect during path construction.
 * 
 * Line join styles may be:
 * 
 * cairo.LINE_JOIN_MITER
 *   use a sharp (angled) corner, see cairo.context_set_miter_limit().
 * cairo.LINE_JOIN_ROUND
 *   use a rounded join, the center of the circle is the joint point.
 * cairo.LINE_JOIN_BEVEL
 *   use a cut-off join, the join is cut off at half the line width from the joint point
 * 
 * The default line join style is cairo.LINE_JOIN_MITER.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} line_join - one of the above line_join values.
 */
static JSVAL context_set_line_join(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    int line_join = args[1]->IntegerValue();
    cairo_set_line_join(context, (cairo_line_join_t)line_join);
    return Undefined();
}

/**
 * @function cairo.context_get_line_join
 * 
 * ### Synopsis
 * 
 * var line_join = cairo.context_get_line_join(context);
 * 
 * Gets the current line join style, as set by cairo.context_set_line_join().
 * 
 * See cairo.context_set_line_join() for description of the line_join values.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} line_join - line join style.
 */
static JSVAL context_get_line_join(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_line_join(context));
}

/**
 * @function cairo.context_set_line_width
 * 
 * ### Synopsis
 * 
 * cairo.context_set_line_width(context, width);
 * 
 * Sets the current line width within the cairo context. The line width value specifies the diameter of a pen that is circular in user space, (though device-space pen may be an ellipse in general due to scaling/shear/rotation of the CTM).
 * 
 * Note: When the description above refers to user space and CTM it refers to the user space and CTM in effect at the time of the stroking operation, not the user space and CTM in effect at the time of the call to cairo_set_line_width(). The simplest usage makes both of these spaces identical. That is, if there is no change to the CTM between a call to cairo_set_line_width() and the stroking operation, then one can just pass user-space values to cairo.context_set_line_width() and ignore this note.
 * 
 * As with the other stroke parameters, the current line width is examined by cairo.context_stroke(), cairo.context_stroke_extents(), and cairo.context_stroke_to_path(), but does not have any effect during path construction.
 * 
 * The default line width value is 2.0.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} width - setting for line width.
 */
static JSVAL context_set_line_width(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double width = args[1]->NumberValue();
    cairo_set_line_width(context, width);
    return Undefined();
}

/**
 * @function cairo.context_get_line_width
 * 
 * ### Synopsis
 * 
 * var width = cairo.context_get_line_width(context);
 * 
 * This function returns the current line width value exactly as set by cairo.context_set_line_width(). 
 * 
 * Note that the value is unchanged even if the CTM has changed between the calls to cairo.context_set_line_width() and cairo.context_get_line_width().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {number} width - current line width.
 */
static JSVAL context_get_line_width(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Number::New(cairo_get_line_width(context));
}

/**
 * @function cairo.context_set_miter_limit
 * 
 * ### Synopsis
 * 
 * cairo.context_set_miter_limit(context, limit);
 * 
 * Sets the current miter limit within the cairo context.
 * 
 * If the current line join style is set to CAIRO_LINE_JOIN_MITER (see cairo.context_set_line_join()), the miter limit is used to determine whether the lines should be joined with a bevel instead of a miter. Cairo divides the length of the miter by the line width. If the result is greater than the miter limit, the style is converted to a bevel.
 * 
 * As with the other stroke parameters, the current line miter limit is examined by cairo.context_stroke(), cairo.context_stroke_extents(), and cairo.context_stroke_to_path(), but does not have any effect during path construction.
 * 
 * The default miter limit value is 10.0, which will convert joins with interior angles less than 11 degrees to bevels instead of miters. For reference, a miter limit of 2.0 makes the miter cutoff at 60 degrees, and a miter limit of 1.414 makes the cutoff at 90 degrees.
 * 
 * A miter limit for a desired angle can be computed as: miter limit = 1/sin(angle/2)
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} limit - setting for miter limit.
 */
static JSVAL context_set_miter_limit(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double limit = args[1]->NumberValue();
    cairo_set_miter_limit(context, limit);
    return Undefined();
}

/**
 * @function cairo.context_get_miter_limit
 * 
 * ### Synopsis
 * 
 * var limit = cairo.context_get_miter_limit(context);
 * 
 * This function returns the current miter limit value exactly as set by cairo.context_set_miter_limit(). 
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {number} width - current line width.
 */
static JSVAL context_get_miter_limit(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Number::New(cairo_get_miter_limit(context));
}

/**
 * @function cairo.context_set_operator
 * 
 * ### Synopsis
 * 
 * cairo.context_set_operator(context, operator);
 * 
 * Sets the compositing operator to be used for all drawing operations.
 * 
 * The default operator is CAIRO_OPERATOR_OVER.
 * 
 * The operator parameter may be one of the following values:
 * 
 * cairo.OPERATOR_CLEAR - clear destination layer (bounded)
 * cairo.OPERATOR_SOURCE - replace destination layer (bounded)
 * cairo.OPERATOR_OVER - draw source layer on top of destination layer (bounded)
 * cairo.OPERATOR_IN - draw source where there was destination content (unbounded)
 * cairo.OPERATOR_OUT - draw source where there was no destination content (unbounded)
 * cairo.OPERATOR_ATOP - draw source on top of destination content and only there
 * cairo.OPERATOR_DEST - ignore the source
 * cairo.OPERATOR_DEST_OVER - draw destination on top of source
 * cairo.OPERATOR_DEST_IN - leave destination only where there was source content (unbounded)
 * cairo.OPERATOR_DEST_OUT - leave destination only where there was no source content
 * cairo.OPERATOR_DEST_ATOP - leave destination on top of source content and only there (unbounded)
 * cairo.OPERATOR_XOR - source and destination are shown where there is only one of them
 * cairo.OPERATOR_ADD - source and destination layers are accumulated
 * cairo.OPERATOR_SATURATE - like over, but assuming source and dest are disjoint geometries
 * cairo.OPERATOR_MULTIPLY - source and destination layers are multiplied. This causes the result to be at least as dark as the darker inputs.
 * cairo.OPERATOR_SCREEN - source and destination are complemented and multiplied. This causes the result to be at least as light as the lighter inputs.
 * cairo.OPERATOR_OVERLAY - multiplies or screens, depending on the lightness of the destination color.
 * cairo.OPERATOR_DARKEN - replaces the destination with the source if it is darker, otherwise keeps the source.
 * cairo.OPERATOR_LIGHTEN - replaces the destination with the source if it is lighter, otherwise keeps the source.
 * cairo.OPERATOR_COLOR_DODGE - brightens the destination color to reflect the source color.
 * cairo.OPERATOR_COLOR_BURN - darkens the destination color to reflect the source color.
 * cairo.OPERATOR_HARD_LIGHT - Multiplies or screens, dependent on source color.
 * cairo.OPERATOR_SOFT_LIGHT - Darkens or lightens, dependent on source color.
 * cairo.OPERATOR_DIFFERENCE - Takes the difference of the source and destination color.
 * cairo.OPERATOR_EXCLUSION - Produces an effect similar to difference, but with lower contrast.
 * cairo.OPERATOR_HSL_HUE - Creates a color with the hue of the source and the saturation and luminosity of the target.
 * cairo.OPERATOR_HSL_SATURATION - Creates a color with the saturation of the source and the hue and luminosity of the target. Painting with this mode onto a gray area produces no change.
 * cairo.OPERATOR_HSL_COLOR - Creates a color with the hue and saturation of the source and the luminosity of the target. This preserves the gray levels of the target and is useful for coloring monochrome images or tinting color images.
 * cairo.OPERATOR_HSL_LUMINOSITY - Creates a color with the luminosity of the source and the hue and saturation of the target. This produces an inverse effect to cairo.OPERATOR_HSL_COLOR.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} operator - one of the above operators.
 */
static JSVAL context_set_operator(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_operator(context, (cairo_operator_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.context_get_operator
 * 
 * ### Synopsis
 * 
 * var operator = cairo.context_get_operator(context);
 * 
 * Gets the current compositing operator for a cairo context.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} operator - one of the operators, as described in cairo.context_set_operator().
 */
static JSVAL context_get_operator(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_operator(context));
}

/**
 * @function cairo.context_set_tolerance
 * 
 * ### Synopsis
 * 
 * cairo.context_set_tolerance(context, tolerance);
 * 
 * Sets the tolerance used when converting paths into trapezoids. 
 * 
 * Curved segments of the path will be subdivided until the maximum deviation between the original path and the polygonal approximation is less than tolerance. 
 * 
 * The default value is 0.1. 
 * 
 * A larger value will give better performance, a smaller value, better appearance. (Reducing the value from the default value of 0.1 is unlikely to improve appearance significantly.) 
 * 
 * The accuracy of paths within Cairo is limited by the precision of its internal arithmetic, and the prescribed tolerance is restricted to the smallest representable internal value.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} tolerance - setting for tolerance, in device units (typically pixels).
 */
static JSVAL context_set_tolerance(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double tolerance = args[1]->NumberValue();
    cairo_set_tolerance(context, tolerance);
    return Undefined();
}

/**
 * @function cairo.context_get_tolerance
 * 
 * ### Synopsis
 * 
 * var tolerance = cairo.context_get_tolerance(context);
 * 
 * This function returns the current tolerance value exactly as set by cairo.context_set_tolerance(). 
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {number} tolerance - current tolerance value.
 */
static JSVAL context_get_tolerance(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Number::New(cairo_get_tolerance(context));
}

/**
 * @function cairo.context_clip
 * 
 * ### Synopsis
 * 
 * cairo.context_clip(context);
 * 
 * Establishes a new clip region by intersecting the current clip region with the current path as it would be filled by cairo.context_fill() and according to the current fill rule (see cairo.context_set_fill_rule()).
 * 
 * After cairo.context_clip(), the current path will be cleared from the cairo context.
 * 
 * The current clip region affects all drawing operations by effectively masking out any changes to the surface that are outside the current clip region.
 * 
 * Calling cairo.context_clip() can only make the clip region smaller, never larger. But the current clip is part of the graphics state, so a temporary restriction of the clip region can be achieved by calling cairo.context_clip() within a cairo.context_save()/cairo.context_restore() pair. The only other means of increasing the size of the clip region is cairo.context_reset_clip().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_clip(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_clip(context);
    return Undefined();
}

/**
 * @function cairo.context_clip_preserve
 * 
 * ### Synopsis
 * 
 * cairo.context_clip_preserve(context);
 * 
 * Establishes a new clip region by intersecting the current clip region with the current path as it would be filled by cairo.context_fill() and according to the current fill rule (see cairo.context_set_fill_rule()).
 * 
 * Unlike cairo.context_clip(), cairo.context_clip_preserve() preserves the path within the cairo context.
 * 
 * The current clip region affects all drawing operations by effectively masking out any changes to the surface that are outside the current clip region.
 * 
 * Calling cairo.context_clip_preserve() can only make the clip region smaller, never larger. But the current clip is part of the graphics state, so a temporary restriction of the clip region can be achieved by calling cairo.context_clip_preserve() within a cairo.context_save()/cairo.context_restore() pair. The only other means of increasing the size of the clip region is cairo.context_reset_clip().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_clip_preserve(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_clip_preserve(context);
    return Undefined();
}

/**
 * @function cairo.context_clip_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_clip_extents(context);
 * 
 * Computes a bounding box in user coordinates covering the area inside the current clip.
 * 
 * The object returned has the following members:
 * 
 * + x1: x coordinate of the top left corner of the resulting extents.
 * + y1: y coordinate of the top left corner of the resulting extents.
 * + x2: x coordinate of the lower right corner of the resulting extents.
 * + y2: y coordinate of the lower right corner of the resulting extents.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} extents - see object description above.
 */
static JSVAL context_clip_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x1,y1, x2,y2;
    cairo_clip_extents(context, &x1,&y1, &x2,&y2);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    o->Set(String::New("x2"), Number::New(x2));
    o->Set(String::New("y2"), Number::New(y2));
    return o;
}

/**
 * @function cairo.context_in_clip
 * 
 * ### Synopsis
 * 
 * var inClip = cairo.context_in_clip(context, x, y);
 * 
 * Tests whether the given point is inside the area that would be visible through the current clip, i.e. the area that would be filled by a cairo.context_paint() operation.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {boolean} inClip - true if the point is inside the clip rectangle, false if not.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL context_in_clip(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return cairo_in_clip(context, args[1]->NumberValue(), args[2]->NumberValue()) ? True() : False();
}
#endif

/**
 * @function cairo.context_reset_clip
 * 
 * ### Synopsis
 * 
 * cairo.context_reset_clip(context);
 * 
 * Reset the current clip region to its original, unrestricted state. 
 * 
 * That is, set the clip region to an infinitely large shape containing the target surface. Equivalently, if infinity is too hard to grasp, one can imagine the clip region being reset to the exact bounds of the target surface.
 * 
 * Note that code meant to be reusable should not call cairo.context_reset_clip() as it will cause results unexpected by higher-level code which calls cairo.context_clip(). Consider using cairo.context_save() and cairo.context_restore() around cairo.context_clip() as a more robust means of temporarily restricting the clip region.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_reset_clip(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_reset_clip(context);
    return Undefined();
}

// rectangle list stuff not implemented (yet)

/**
 * @function cairo.context_fill
 * 
 * ### Synopsis
 * 
 * cairo.context_fill(context);
 * 
 * A drawing operator that fills the current path according to the current fill rule, (each sub-path is implicitly closed before being filled). 
 * 
 * After cairo.context_fill(), the current path will be cleared from the cairo context. 
 * 
 * See cairo.context_set_fill_rule() and cairo.context_fill_preserve().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_fill(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_fill(context);
    return Undefined();
}

/**
 * @function cairo.context_fill_preserve
 * 
 * ### Synopsis
 * 
 * cairo.context_fill_preserve(context);
 * 
 * A drawing operator that fills the current path according to the current fill rule, (each sub-path is implicitly closed before being filled). 
 * 
 * Unlike cairo.context_fill(), cairo.context_fill_preserve() preserves the path within the cairo context.
 * 
 * See cairo.context_set_fill_rule() and cairo.context_fill().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_fill_preserve(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_fill_preserve(context);
    return Undefined();
}

/**
 * @function cairo.context_fill_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_fill_extents(context);
 * 
 * Computes a bounding box in user coordinates covering the area that would be affected, (the "inked" area), by a cairo.context_fill() operation given the current path and fill parameters. If the current path is empty, returns an empty rectangle ((0,0), (0,0)). Surface dimensions and clipping are not taken into account.
 * 
 * Contrast with cairo.context_path_extents(), which is similar, but returns non-zero extents for some paths with no inked area, (such as a simple line segment).
 * 
 * Note that cairo.context_fill_extents() must necessarily do more work to compute the precise inked areas in light of the fill rule, so cairo.context_path_extents() may be more desirable for sake of performance if the non-inked path extents are desired.
 * 
 * See cairo.context_fill(), cairo.context_set_fill_rule() and cairo.context_fill_preserve().
 * 
 * The object returned has the following members:
 * 
 * + x1: x coordinate of the top left corner of the resulting extents.
 * + y1: y coordinate of the top left corner of the resulting extents.
 * + x2: x coordinate of the lower right corner of the resulting extents.
 * + y2: y coordinate of the lower right corner of the resulting extents.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} extents - see object description above.
 */
static JSVAL context_fill_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x1,y1, x2,y2;
    cairo_fill_extents(context, &x1,&y1, &x2,&y2);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    o->Set(String::New("x2"), Number::New(x2));
    o->Set(String::New("y2"), Number::New(y2));
    return o;
}

/**
 * @function cairo.context_in_fill
 * 
 * ### Synopsis
 * 
 * var inFill = cairo.context_in_fill(context, x, y);
 * 
 * Tests whether the given point is inside the area that would be affected by a cairo.context_fill() operation given the current path and filling parameters. Surface dimensions and clipping are not taken into account.
 * 
 * See cairo.context_fill(), cairo.context_set_fill_rule() and cairo.context_fill_preserve().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {boolean} inFill - true if the point is inside the area that would be affected, false if not.
 */
static JSVAL context_in_fill(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return cairo_in_fill(context, args[1]->NumberValue(), args[2]->NumberValue()) ? True() : False();
}

/**
 * @function cairo.context_mask
 * 
 * ### Synopsis
 * 
 * cairo.context_mask(context, pattern);
 * 
 * A drawing operator that paints the current source using the alpha channel of pattern as a mask. 
 * 
 * Opaque areas of pattern are painted with the source, transparent areas are not painted.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL context_mask(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[1]);
    cairo_mask(context, pattern);
    return Undefined();
}

/**
 * @function cairo.context_mask_surface
 * 
 * ### Synopsis
 * 
 * cairo.context_mask_surface(context, surface, surface_x, surface_y);
 * 
 * A drawing operator that paints the current source using the alpha channel of surface as a mask. 
 * 
 * Opaque areas of surface are painted with the source, transparent areas are not painted.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {number} surface_x - x coordinate at which to place the origin of surface.
 * @param {number} surface_y - y coordinate at which to place the origin of surface.
 */
static JSVAL context_mask_surface(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[1]);
    cairo_mask_surface(context, surface, args[2]->NumberValue(), args[3]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_paint
 * 
 * ### Synopsis
 * 
 * cairo.context_paint(context);
 * 
 * A drawing operator that paints the current source everywhere within the current clip region.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_paint(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_paint(context);
    return Undefined();
}

/**
 * @function cairo.context_paint_with_alpha
 * 
 * ### Synopsis
 * 
 * cairo.context_paint_with_alpha(context, alpha);
 * 
 * A drawing operator that paints the current source everywhere within the current clip region using a mask of constant alpha value alpha. 
 * 
 * The effect is similar to cairo.context_paint(), but the drawing is faded out using the alpha value.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} alpha - alpha value, between 0 (transparent) and 1 (opaque).
 */
static JSVAL context_paint_with_alpha(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_paint_with_alpha(context, args[1]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_stroke
 * 
 * ### Synopsis
 * 
 * cairo.context_stroke(context);
 * 
 * A drawing operator that strokes the current path according to the current line width, line join, line cap, and dash settings. After cairo.context_stroke(), the current path will be cleared from the cairo context. 
 * 
 * See cairo.context_set_line_width(), cairo.context_set_line_join(), cairo.context_set_line_cap(), cairo.context_set_dash(), and cairo.context_stroke_preserve().
 * 
 * Note: Degenerate segments and sub-paths are treated specially and provide a useful result. These can result in two different situations:
 * 
 * 1. Zero-length "on" segments set in cairo.context_set_dash(). If the cap style is cairo.LINE_CAP_ROUND or cairo.LINE_CAP_SQUARE then these segments will be drawn as circular dots or squares respectively. In the case of cairo.LINE_CAP_SQUARE, the orientation of the squares is determined by the direction of the underlying path.
 * 2. A sub-path created by cairo.context_move_to() followed by either a cairo.context_close_path() or one or more calls to cairo.context_line_to() to the same coordinate as the cairo_move_to(). If the cap style is cairo._LINE_CAP_ROUND then these sub-paths will be drawn as circular dots. Note that in the case of cairo.LINE_CAP_SQUARE a degenerate sub-path will not be drawn at all, (since the correct orientation is indeterminate).
 * 
 * In no case will a cap style of cairo.LINE_CAP_BUTT cause anything to be drawn in the case of either degenerate segments or sub-paths.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_stroke(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_stroke(context);
    return Undefined();
}

/**
 * @function cairo.context_stroke_preserve
 * 
 * ### Synopsis
 * 
 * cairo.stroke_preserve(context);
 * 
 * A drawing operator that strokes the current path according to the current line width, line join, line cap, and dash settings. 
 * 
 * Unlike cairo.context_stroke(), cairo.context_stroke_preserve() preserves the path within the cairo context.
 * 
 * See cairo.context_set_line_width(), cairo.context_set_line_join(), cairo.context_set_line_cap(), and cairo.context_set_dash().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_stroke_preserve(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_stroke_preserve(context);
    return Undefined();
}

/**
 * @function cairo.context_stroke_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_stroke_extents(context);
 * 
 * Computes a bounding box in user coordinates covering the area that would be affected, (the "inked" area), by a cairo.context_stroke() operation given the current path and stroke parameters. If the current path is empty, returns an empty rectangle ((0,0), (0,0)). Surface dimensions and clipping are not taken into account.
 * 
 * Note that if the line width is set to exactly zero, then cairo.context_stroke_extents() will return an empty rectangle. Contrast with cairo.context_path_extents() which can be used to compute the non-empty bounds as the line width approaches zero.
 * 
 * Note that cairo.context_stroke_extents() must necessarily do more work to compute the precise inked areas in light of the stroke parameters, so cairo.context_path_extents() may be more desirable for sake of performance if non-inked path extents are desired.
 * 
 * See cairo.context_stroke(), cairo.context_set_line_width(), cairo.context_set_line_join(), cairo.context_set_line_cap(), cairo.context_set_dash(), and cairo.context_stroke_preserve().
 * 
 * The object returned has the following members:
 * 
 * + x1: x coordinate of the top left corner of the resulting extents.
 * + y1: y coordinate of the top left corner of the resulting extents.
 * + x2: x coordinate of the lower right corner of the resulting extents.
 * + y2: y coordinate of the lower right corner of the resulting extents.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} extents - see object description above.
 */
static JSVAL context_stroke_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x1,y1, x2,y2;
    cairo_stroke_extents(context, &x1,&y1, &x2,&y2);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    o->Set(String::New("x2"), Number::New(x2));
    o->Set(String::New("y2"), Number::New(y2));
    return o;
}

/**
 * @function cairo.context_in_stroke
 * 
 * ### Synopsis
 * 
 * var inStroke = cairo.context_in_stroke(context, x, y);
 * 
 * Tests whether the given point is inside the area that would be affected by a cairo.context_stroke() operation given the current path and stroking parameters. 
 * 
 * Surface dimensions and clipping are not taken into account.
 * 
 * See cairo.context_stroke(), cairo.context_set_line_width(), cairo.context_set_line_join(), cairo.context_set_line_cap(), cairo.context_set_dash(), and cairo.context_stroke_preserve().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {boolean} inStroke - true if the point is inside the area that would be affected, false if not.
 */
static JSVAL context_in_stroke(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return cairo_in_stroke(context, args[1]->NumberValue(), args[2]->NumberValue()) ? True() : False();
}

/**
 * @function cairo.context_copy_page
 * 
 * ### Synopsis
 * 
 * cairo.context_copy_page(context);
 * 
 * Emits the current page for backends that support multiple pages, but doesn't clear it, so, the contents of the current page will be retained for the next page too. Use cairo.conext_show_page() if you want to get an empty page after the emission.
 * 
 * This is a convenience function that simply calls cairo.surface_copy_page() on context's target.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_copy_page(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_copy_page(context);
    return Undefined();
}

/**
 * @function cairo.context_show_page
 * 
 * ### Synopsis
 * 
 * cairo.context_show_page(context);
 * 
 * Emits and clears the current page for backends that support multiple pages. Use cairo.context_copy_page() if you don't want to clear the page.
 * 
 * This is a convenience function that simply calls cairo.surface_show_page() on context's target.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_show_page(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_show_page(context);
    return Undefined();
}

/**
 * @function cairo.context_translate
 * 
 * ### Synopsis
 * 
 * cairo.context_translate(context, tx,ty);
 * 
 * Modifies the current transformation matrix (CTM) by translating the user-space origin by (tx, ty). 
 * 
 * This offset is interpreted as a user-space coordinate according to the CTM in place before the new call to cairo.context_translate(). 
 * 
 * In other words, the translation of the user-space origin takes place after any existing transformation.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} tx - amount to translate in the x direction.
 * @param {number} ty - amount to translate in the y direction.
 */
static JSVAL context_translate(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_translate(context, args[1]->NumberValue(), args[2]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_scale
 * 
 * ### Synopsis
 * 
 * cairo.context_scale(context, sx,sy);
 * 
 * Modifies the current transformation matrix (CTM) by scaling the X and Y user-space axes by sx and sy respectively. 
 * 
 * The scaling of the axes takes place after any existing transformation of user space.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} sx - scale factor for the x dimension.
 * @param {number} sy - scale factor for the y dimension.
 */
static JSVAL context_scale(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_scale(context, args[1]->NumberValue(), args[2]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_rotate
 * 
 * ### Synopsis
 * 
 * cairo.context_rotate(context, angle);
 * 
 * Modifies the current transformation matrix (CTM) by rotating the user-space axes by angle radians. 
 * 
 * The rotation of the axes takes places after any existing transformation of user space. 
 * 
 * The rotation direction for positive angles is from the positive X axis toward the positive Y axis.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} angle - angle in radians by which the user space axes will be rotated.
 */
static JSVAL context_rotate(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_rotate(context, args[1]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_transform
 * 
 * ### Synopsis
 * 
 * cairo.context_transform(context, matrix);
 * 
 * Modifies the current transformation matrix (CTM) by applying matrix as an additional transformation. 
 * 
 * The new transformation of user space takes place after any existing transformation.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} matrix - opaque handle to a cairo matrix.
 */
static JSVAL context_transform(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[1]);
    cairo_transform(context, matrix);
    return Undefined();
}

/**
 * @function cairo.context_set_matrix
 * 
 * ### Synopsis
 * 
 * cairo.context_set_matrix(pattern, matrix);
 * 
 * Modifies the current transformation matrix (CTM) by setting it equal to matrix.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} matrix - opaque handle to a cairo matrix.
 */
static JSVAL context_set_matrix(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[1]);
    cairo_set_matrix(context, matrix);
    return Undefined();
}


/**
 * @function cairo.context_get_matrix
 * 
 * ### Synopsis
 * 
 * var matrix = cairo.context_get_matrix(context);
 * 
 * Gets the context's transformation matrix.
 * 
 * The matrix returned is owned by the caller and must be released by calling cairo.matrix_destroy() when it is no longer needed.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} matrix - opaque handle to a cairo matrix.
 */
static JSVAL context_get_matrix(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_matrix_t *matrix = new cairo_matrix_t;
    cairo_get_matrix(context, matrix);
    return External::New(matrix);
}

/**
 * @function cairo.context_identity_matrix
 * 
 * ### Synopsis
 * 
 * cairo.context_identity_matrix(context);
 * 
 * Resets the current transformation matrix (CTM) by setting it equal to the identity matrix. 
 * 
 * That is, the user-space and device-space axes will be aligned and one user-space unit will transform to one device-space unit.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_identity_matrix(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_identity_matrix(context);
    return Undefined();
}

/**
 * @function cairo.context_user_to_device
 * 
 * ### Synopsis
 * 
 * var vector = cairo.context_user_to_device(context, vector);
 * 
 * Transform a coordinate from user space to device space by multiplying the given point by the current transformation matrix (CTM).
 * 
 * The input vector and output vector are the same object, of the form:
 * 
 * + {number} x - x coordinate.
 * + {number} y - y coordinate
 * 
 * The dx,dy members are modified by this routine.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} vector - JavaScript object of the form described above.
 * @return {object} vector - JavaScript object of the form described above.
 */
static JSVAL context_user_to_device(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    JSOBJ o = args[1]->ToObject();
    double x = o->Get(_x)->NumberValue();
    double y = o->Get(_y)->NumberValue();
    cairo_user_to_device(context, &x, &y);
    o->Set(_x, Number::New(x));
    o->Set(_y, Number::New(y));
    return o;
}

/**
 * @function cairo.context_user_to_device_distance
 * 
 * ### Synopsis
 * 
 * var vector = cairo.context_user_to_device_distance(context, vector);
 * 
 * Transform a distance vector from user space to device space. 
 * 
 * This function is similar to cairo.context_user_to_device() except that the translation components of the CTM will be ignored when transforming (dx,dy).
 * 
 * The input vector and output vector are the same object, of the form:
 * 
 * + {number} dx - x component of distance vector.
 * + {number} dy - y component of distance vector.
 * 
 * The dx,dy members are modified by this routine.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} vector - JavaScript object of the form described above.
 * @return {object} vector - JavaScript object of the form described above.
 */
static JSVAL context_user_to_device_distance(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Local<String>_dx = String::New("dx");
    Local<String>_dy = String::New("dy");
    JSOBJ o = args[1]->ToObject();
    double dx = o->Get(_dx)->NumberValue();
    double dy = o->Get(_dy)->NumberValue();
    cairo_user_to_device_distance(context, &dx, &dy);
    o->Set(_dx, Number::New(dx));
    o->Set(_dy, Number::New(dy));
    return o;
}

/**
 * @function cairo.context_device_to_user
 * 
 * ### Synopsis
 * 
 * var vector = cairo.context_device_to_user(context, vector);
 * 
 * Transform a coordinate from device space to user space by multiplying the given point by the inverse of the current transformation matrix (CTM).
 * 
 * The input vector and output vector are the same object, of the form:
 * 
 * + {number} x - x component of distance vector.
 * + {number} y - y component of distance vector.
 * 
 * The dx,dy members are modified by this routine.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} vector - JavaScript object of the form described above.
 * @return {object} vector - JavaScript object of the form described above.
 */
static JSVAL context_device_to_user(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    JSOBJ o = args[1]->ToObject();
    double x = o->Get(_x)->NumberValue();
    double y = o->Get(_y)->NumberValue();
    cairo_device_to_user(context, &x, &y);
    o->Set(_x, Number::New(x));
    o->Set(_y, Number::New(y));
    return o;
}

/**
 * @function cairo.context_device_to_user_distance
 * 
 * ### Synopsis
 * 
 * var vector = cairo.context_device_to_user_distance(context, vector);
 * 
 * Transform a distance vector from device space to user space. 
 * 
 * This function is similar to cairo.context_device_to_user() except that the translation components of the inverse CTM will be ignored when transforming (dx,dy).
 * 
 * The input vector and output vector are the same object, of the form:
 * 
 * + {number} dx - x component of distance vector.
 * + {number} dy - y component of distance vector.
 * 
 * The dx,dy members are modified by this routine.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} vector - JavaScript object of the form described above.
 * @return {object} vector - JavaScript object of the form described above.
 */
static JSVAL context_device_to_user_distance(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Local<String>_dx = String::New("dx");
    Local<String>_dy = String::New("dy");
    JSOBJ o = args[1]->ToObject();
    double dx = o->Get(_dx)->NumberValue();
    double dy = o->Get(_dy)->NumberValue();
    cairo_device_to_user_distance(context, &dx, &dy);
    o->Set(_dx, Number::New(dx));
    o->Set(_dy, Number::New(dy));
    return o;
}



////////////////////////// PATHS
// http://www.cairographics.org/manual/cairo-Paths.html

// functions to return cario_path_t and manipulate them not done.


/**
 * @function cairo.context_copy_path
 * 
 * ### Synopsis
 * 
 * var path = cairo.context_copy_path(context);
 * 
 * Creates a copy of the current path.
 * 
 * The caller owns the returned object and should call cairo.path_destroy() when finished with it.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} path - opaque handle to a cairo path
 */
static JSVAL context_copy_path(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_copy_path(context));
}

/**
 * @function cairo.context_copy_path_flat
 * 
 * ### Synopsis
 * 
 * var path = cairo.context_copy_path_flat(context);
 * 
 * Creates a copy of the current path.
 * 
 * Gets a flattened copy of the current path and returns it to the user as a cairo_path_t. See cairo_path_data_t for hints on how to iterate over the returned data structure.
 * 
 * This function is like cairo.context_copy_path() except that any curves in the path will be approximated with piecewise-linear approximations, (accurate to within the current tolerance value). 
 * 
 * That is, the result is guaranteed to not have any elements of type cairo.PATH_CURVE_TO which will instead be replaced by a series of cairo.PATH_LINE_TO elements.
 * 
 * The caller owns the returned object and should call cairo.path_destroy() when finished with it.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} path - opaque handle to a cairo path
 */
static JSVAL context_copy_path_flat(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_copy_path_flat(context));
}

/**
 * @function cairo.context_append_path
 * 
 * ### Synopsis
 * 
 * cairo.context_append_path(context, path);
 * 
 * Append the path onto the current path.
 * 
 * The path may be either the return value from one of cairo.context_copy_path() or cairo.context_copy_path_flat()
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} path - opaque handle to a cairo path
 */
static JSVAL context_append_path(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_path_t *path = (cairo_path_t *)JSEXTERN(args[1]);
    cairo_append_path(context, path);
    return Undefined();
}

/**
 * @functino cairo.path_destroy
 * 
 * ### Synopsis
 * 
 * cairo.path_dstroy(path);
 * 
 * Immediately releases all memory associated with path. 
 * 
 * After a call to cairo.path_destroy() the path pointer is no longer valid and should not be used further.
 * 
 * @param {object} path - opaque handle to a cairo path
 */
static JSVAL path_destroy(JSARGS args) {
    cairo_path_t *path = (cairo_path_t *)JSEXTERN(args[0]);
    cairo_path_destroy(path);
    return Undefined();
}

/**
 * @function cairo.context_has_current_point
 * 
 * ### Synopsis
 * 
 * var hasPoint = cairo.context_has_current_point(context);
 * 
 * Returns whether a current point is defined on the current path. 
 * 
 * See cairo.context_get_current_point() for details on the current point.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {boolean} hasPoint - true if a current point is defined.
 */
static JSVAL context_has_current_point(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return cairo_has_current_point(context) ? True() : False();
}

/**
 * @function cairo.context_get_current_point
 * 
 * ### Synopsis
 * 
 * var point = cairo.context_get_current_point(context);
 * 
 * Gets the current point of the current path, which is conceptually the final point reached by the path so far.
 * 
 * The current point is returned in the user-space coordinate system. If there is no defined current point or if context is in an error status, x and y will both be set to 0.0. It is possible to check this in advance with cairo.context_has_current_point().
 * 
 * Most path construction functions alter the current point. 
 * 
 * See the following for details on how they affect the current point: cairo.context_new_path(), cairo.context_new_sub_path(), cairo.context_append_path(), cairo.context_close_path(), cairo.context_move_to(), cairo.context_line_to(), cairo.context_curve_to(), cairo.context_rel_move_to(), cairo.context_rel_line_to(), cairo.context_rel_curve_to(), cairo.context_arc(), cairo.context_arc_negative(), cairo.context_rectangle(), cairo.context_text_path(), cairo.context_glyph_path(), cairo_stroke_to_path().
 * 
 * Some functions use and alter the current point but do not otherwise change current path: cairo.context_show_text().
 * 
 * Some functions unset the current path and as a result, current point: cairo.context_fill(), cairo.context_stroke().
 * 
 * The object returned has just two members:
 * 
 * + {number} x: x coordinate of the current point
 * + {number} y: y coordinate of the current point
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} point - object of the form described above.
 */
static JSVAL context_get_current_point(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x,y;
    
    cairo_get_current_point(context, &x, &y);
    JSOBJ o = Object::New();
    o->Set(String::New("x"), Number::New(x));
    o->Set(String::New("y"), Number::New(y));
    return o;
}

/**
 * @function cairo.context_new_path
 * 
 * ### Synopsis
 * 
 * cairo.context_new_path(context);
 * 
 * Clears the current path. After this call there will be no path and no current point.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_new_path(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_new_path(context);
    return Undefined();
}

/**
 * @function cairo.context_new_sub_path
 * 
 * ### Synopsis
 * 
 * cairo.context_new_sub_path(context);
 * 
 * Begin a new sub-path. 
 * 
 * Note that the existing path is not affected. After this call there will be no current point.
 * 
 * In many cases, this call is not needed since new sub-paths are frequently started with cairo.context_move_to().
 * 
 * A call to cairo.context_new_sub_path() is particularly useful when beginning a new sub-path with one of the cairo.context_arc() calls. This makes things easier as it is no longer necessary to manually compute the arc's initial coordinates for a call to cairo.context_move_to().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_new_sub_path(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_new_sub_path(context);
    return Undefined();
}

/**
 * @function cairo.context_close_path
 * 
 * ### Synopsis
 * 
 * cairo.context_close_path(context);
 * 
 * Adds a line segment to the path from the current point to the beginning of the current sub-path, (the most recent point passed to cairo.context_move_to()), and closes this sub-path. After this call the current point will be at the joined endpoint of the sub-path.
 * 
 * The behavior of cairo.context_close_path() is distinct from simply calling cairo.context_line_to() with the equivalent coordinate in the case of stroking. When a closed sub-path is stroked, there are no caps on the ends of the sub-path. Instead, there is a line join connecting the final and initial segments of the sub-path.
 * 
 * If there is no current point before the call to cairo.context_close_path(), this function will have no effect.
 * 
 * Note: As of cairo version 1.2.4 any call to cairo.context_close_path() will place an explicit MOVE_TO element into the path immediately after the CLOSE_PATH element, (which can be seen in cairo.context_copy_path() for example). This can simplify path processing in some cases as it may not be necessary to save the "last move_to point" during processing as the MOVE_TO immediately after the CLOSE_PATH will provide that point.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_close_path(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_close_path(context);
    return Undefined();
}

/**
 * @function cairo.context_arc
 * 
 * ### Synopsis
 * 
 * cairo.context_arc(context, xc,yc, radius, angle1, angle2);
 * 
 * Adds a circular arc of the given radius to the current path. 
 * 
 * The arc is centered at (xc, yc), begins at angle1 and proceeds in the direction of increasing angles to end at angle2. If angle2 is less than angle1 it will be progressively increased by 2*M_PI until it is greater than angle1.
 * 
 * If there is a current point, an initial line segment will be added to the path to connect the current point to the beginning of the arc. If this initial line is undesired, it can be avoided by calling cairo.context__new_sub_path() before calling cairo.context_arc().
 * 
 * Angles are measured in radians. An angle of 0.0 is in the direction of the positive X axis (in user space). An angle of M_PI/2.0 radians (90 degrees) is in the direction of the positive Y axis (in user space). Angles increase in the direction from the positive X axis toward the positive Y axis. So with the default transformation matrix, angles increase in a clockwise direction.
 * 
 * (To convert from degrees to radians, use degrees * (M_PI / 180.).)
 * 
 * This function gives the arc in the direction of increasing angles; see cairo.context_arc_negative() to get the arc in the direction of decreasing angles.
 * 
 * The arc is circular in user space. To achieve an elliptical arc, you can scale the current transformation matrix by different amounts in the X and Y directions. For example, to draw an ellipse in the box given by x, y, width, height:
 * 
 * ```
 * cairo_save (context);
 * cairo_translate (context, x + width / 2., y + height / 2.);
 * cairo_scale (context, width / 2., height / 2.);
 * cairo_arc (context, 0., 0., 1., 0., 2 * M_PI);
 * cairo_restore (context);
 * ```
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} xc - x coordinate of the center of the arc.
 * @param {number} yc - y coordinate of the center of the arc.
 * @param {number} radius - radius of the arc.
 * @param {number} angle1 - start angle in radians.
 * @param {number} angle2 - end angle in radians.
 */
static JSVAL context_arc(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_arc(context,
              args[1]->NumberValue(),   // xc
              args[2]->NumberValue(),   // yc
              args[3]->NumberValue(),   // radius
              args[4]->NumberValue(),   // angle1
              args[5]->NumberValue()   // angle2
    );
    return Undefined();
}

/**
 * @function cairo.context_arc_negative
 * 
 * ### Synopsis
 * 
 * cairo.context_arc_negative(context, xc,yc, radius, angle1, angle2);
 * 
 * Adds a circular arc of the given radius to the current path. 
 * 
 * The arc is centered at (xc, yc), begins at angle1 and proceeds in the direction of decreasing angles to end at angle2. If angle2 is greater than angle1 it will be progressively decreased by 2*M_PI until it is less than angle1.
 * 
 * See cairo.context_arc() for more details. This function differs only in the direction of the arc between the two angles.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} xc - x coordinate of the center of the arc.
 * @param {number} yc - y coordinate of the center of the arc.
 * @param {number} radius - radius of the arc.
 * @param {number} angle1 - start angle in radians.
 * @param {number} angle2 - end angle in radians.
 */
static JSVAL context_arc_negative(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_arc_negative(context,
              args[1]->NumberValue(),   // xc
              args[2]->NumberValue(),   // yc
              args[3]->NumberValue(),   // radius
              args[4]->NumberValue(),   // angle1
              args[5]->NumberValue()   // angle2
    );
    return Undefined();
}

/**
 * @function cairo.context_curve_to
 * 
 * ### Synopsis
 * 
 * cairo.context_curve_to(context, x1,y1, x2,y2, x3,y3);
 * 
 * Adds a cubic Bézier spline to the path from the current point to position (x3, y3) in user-space coordinates, using (x1, y1) and (x2, y2) as the control points. 
 * 
 * After this call the current point will be (x3, y3).
 * 
 * If there is no current point before the call to cairo_curve_to() this function will behave as if preceded by a call to cairo.context_move_to(cr, x1, y1).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} x1 - x coordinate of the first control point.
 * @param {number} y1 - y coordinate of the first control point.
 * @param {number} x2 - x coordinate of the second control point.
 * @param {number} y2 - y coordinate of the second control point.
 * @param {number} x3 - x coordinate of the third control point.
 * @param {number} y3 - y coordinate of the third control point.
 */
static JSVAL context_curve_to(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_curve_to(context,
              args[1]->NumberValue(),   // x1
              args[2]->NumberValue(),   // y1
              args[3]->NumberValue(),   // x2
              args[4]->NumberValue(),   // y2
              args[5]->NumberValue(),   // x3
              args[6]->NumberValue()   // y3
    );
    return Undefined();
}

/**
 * @function cairo.context_line_to
 * 
 * ### Synopsis
 * 
 * cairo.context_line_to(context, x,y);
 * 
 * Adds a line to the path from the current point to position (x, y) in user-space coordinates. 
 * 
 * After this call the current point will be (x, y).
 * 
 * If there is no current point before the call to cairo.context_line_to() this function will behave as cairo.context_move_to(context, x, y).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} x - x coordinate of the end of the new line.
 * @param {number} y - y coordinate of the end of the new line.
 */
static JSVAL context_line_to(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_line_to(context,
              args[1]->NumberValue(),   // x
              args[2]->NumberValue()   // y
    );
    return Undefined();
}

/**
 * @function cairo.context_move_to
 * 
 * ### Synopsis
 * 
 * cairo.context_move_to(context, x,y);
 * 
 * Adds a line to the path from the current point to position (x, y) in user-space coordinates. 
 * 
 * After this call the current point will be (x, y).
 * 
 * If there is no current point before the call to cairo.context_line_to() this function will behave as cairo.context_move_to(context, x, y).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} x - x coordinate of the end of the new position.
 * @param {number} y - y coordinate of the end of the new position.
 */
static JSVAL context_move_to(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    
    cairo_move_to(context,
              args[1]->NumberValue(),   // x
              args[2]->NumberValue()   // y
    );
    return Undefined();
}

/**
 * @function cairo.context_rectangle
 * 
 * ### Synopsis
 * 
 * cairo.context_rectangle(context, x,y, w,h);
 * 
 * Adds a closed sub-path rectangle of the given size to the current path at position (x, y) in user-space coordinates.
 * 
 * This is logically equivalent to:
 * 
 * ```
 * cairo_move_to context, x, y);
 * cairo_rel_line_to (context, width, 0);
 * cairo_rel_line_to context, 0, height);
 * cairo_rel_line_to (context, -width, 0);
 * cairo_close_path (context);
 * ```
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} x - x coordinate of the top left corner of the rectangle.
 * @param {number} y - y coordinate of the top left corner of the rectangle.
 * @param {number} w - width of the rectangle.
 * @param {number} h - height of the rectangle.
 */
static JSVAL context_rectangle(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    
    cairo_rectangle(context,
              args[1]->NumberValue(),  // x
              args[2]->NumberValue(),  // y
              args[3]->NumberValue(),  // w
              args[4]->NumberValue()   // h
    );
    return Undefined();
}

/**
 * @function cairo.context_glyph_path
 * 
 * ### Synopsis
 * 
 * cairo.context_glyph_path(context, glyphs);
 * 
 * Adds closed paths for the glyphs to the current path. 
 * 
 * The generated path if filled, achieves an effect similar to that of cairo.context_show_glyphs().
 * 
 * The glyphs argument is an array of objects of the following form:
 * 
 * + {int} index - glyph index in the font.  The exact interpretation of the glyph index depends on the font technology being used.
 * + {number} x - the offset in the x direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * + {number} y - the offset in the y direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * 
 * The above structure holds information about a single glyph when drawing or measuring text. A font is (in simple terms) a collection of shapes used to draw text. A glyph is one of these shapes. There can be multiple glyphs for a single character (alternates to be used in different contexts, for example), or a glyph can be a ligature of multiple characters. Cairo doesn't expose any way of converting input text into glyphs, so in order to use the Cairo interfaces that take arrays of glyphs, you must directly access the appropriate underlying font system.
 * 
 * Note that the offsets given by x and y are not cumulative. When drawing or measuring text, each glyph is individually positioned with respect to the overall origin
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {array} glyphs - array of objects as described above.
 */
static JSVAL context_glyph_path(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Handle<Array>glyphs = Handle<Array>::Cast(args[1]->ToObject());
    int num_glyphs = glyphs->Length();
    cairo_glyph_t *c_glyphs = new cairo_glyph_t[num_glyphs];
    // 
    Local<String>index = String::New("index");
    Local<String>x = String::New("x");
    Local<String>y = String::New("y");
    
    for (int i=0; i<num_glyphs; i++) {
        JSOBJ o = glyphs->Get(i)->ToObject();
        c_glyphs[i].index = o->Get(index)->IntegerValue();
        c_glyphs[i].x = o->Get(x)->NumberValue();
        c_glyphs[i].y = o->Get(y)->NumberValue();
    }
    cairo_glyph_path(context, c_glyphs, num_glyphs);
    delete [] c_glyphs;
    return Undefined();
}

/**
 * @function cairo.context_text_path
 * 
 * ### Synopsis
 * 
 * cairo.context_text_path(context, str);
 * 
 * Adds closed paths for text to the current path. 
 * 
 * The generated path if filled, achieves an effect similar to that of cairo.context__show_text().
 * 
 * Text conversion and positioning is done similar to cairo.context_show_text().
 * 
 * Like cairo.context_show_text(), After this call the current point is moved to the origin of where the next glyph would be placed in this same progression. That is, the current point will be at the origin of the final glyph offset by its advance values. This allows for chaining multiple calls to to cairo.context_text_path() without having to set current point in between.
 * 
 * Note: The cairo.context_text_path() function call is part of what the cairo designers call the "toy" text API. It is convenient for short demos and simple programs, but it is not expected to be adequate for serious text-using applications. See cairo.context_glyph_path() for the "real" text path API in cairo.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {string} str - text
 */
static JSVAL context_text_path(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    String::Utf8Value str(args[1]->ToString());
    cairo_text_path(context, *str);
    return Undefined();
}

/**
 * @function cairo.context_rel_curve_to
 * 
 * ### Synopsis
 * 
 * cairo.context_rel_curve_to(context, dx1,dy1, dx2,dy2, dx3,dy3);
 * 
 * Relative-coordinate version of cairo.context_curve_to(). 
 * 
 * All offsets are relative to the current point. Adds a cubic Bézier spline to the path from the current point to a point offset from the current point by (dx3, dy3), using points offset by (dx1, dy1) and (dx2, dy2) as the control points. After this call the current point will be offset by (dx3, dy3).
 * 
 * Given a current point of (x, y), cairo.context_rel_curve_to(context, dx1, dy1, dx2, dy2, dx3, dy3) is logically equivalent to cairo.context_curve_to(context, x+dx1, y+dy1, x+dx2, y+dy2, x+dx3, y+dy3).
 * 
 * It is an error to call this function with no current point. Doing so will cause context to shutdown with a status of cairo.STATUS_NO_CURRENT_POINT.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} dx1 - x offset to the first control point.
 * @param {number} dy1 - y offset to the first control point.
 * @param {number} dx2 - x offset to the second control point.
 * @param {number} dy2 - y offset to the second control point.
 * @param {number} dx3 - x offset to the third control point.
 * @param {number} dy3 - y offset to the third control point.
 */
static JSVAL context_rel_curve_to(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_rel_curve_to(context,
              args[1]->NumberValue(),   // dx1
              args[2]->NumberValue(),   // dy1
              args[3]->NumberValue(),   // dx2
              args[4]->NumberValue(),   // dy2
              args[5]->NumberValue(),   // dx3
              args[6]->NumberValue()    // dy3
    );
    return Undefined();
}

/**
 * @function cairo.context_rel_line_to
 * 
 * ### Synopsis
 * 
 * cairo.context_rel_line_to(context, dx,dy);
 * 
 * Relative-coordinate version of cairo.context__line_to(). 
 * 
 * Adds a line to the path from the current point to a point that is offset from the current point by (dx, dy) in user space. After this call the current point will be offset by (dx, dy).
 * 
 * Given a current point of (x, y), cairo.context_rel_line_to(context, dx, dy) is logically equivalent to cairo.context_line_to(context, x + dx, y + dy).
 * 
 * It is an error to call this function with no current point. Doing so will cause context to shutdown with a status of cairo.STATUS_NO_CURRENT_POINT.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} dx - x offset to the end of the new line.
 * @param {number} dy - y offset to the end of the new line.
 */
static JSVAL context_rel_line_to(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_rel_line_to(context,
              args[1]->NumberValue(),   // dx
              args[2]->NumberValue()    // dy
    );
    return Undefined();
}

/**
 * @function cairo.context_rel_move_to
 * 
 * ### Synopsis
 * 
 * cairo.context_rel_move_to(context, dx,dy);
 * 
 * Begin a new sub-path. 
 * 
 * After this call the current point will offset by (x, y).
 * 
 * Given a current point of (x, y), cairo.context_rel_move_to(context, dx, dy) is logically equivalent to cairo.context_move_to(context, x + dx, y + dy).
 * 
 * It is an error to call this function with no current point. Doing so will cause context to shutdown with a status of cairo.STATUS_NO_CURRENT_POINT.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} dx - x offset to the end of the new position.
 * @param {number} dy - y offset to the end of the new position.
 */
static JSVAL context_rel_move_to(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    
    cairo_rel_move_to(context,
              args[1]->NumberValue(),   // dx
              args[2]->NumberValue()    // dy
    );
    return Undefined();
}

/**
 * @function cairo.context_path_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_path_extents(context);
 * 
 * Computes a bounding box in user-space coordinates covering the points on the current path. 
 * 
 * If the current path is empty, returns an empty rectangle ((0,0), (0,0)). Stroke parameters, fill rule, surface dimensions and clipping are not taken into account.
 * 
 * Contrast with cairo.context_fill_extents() and cairo.context_stroke_extents() which return the extents of only the area that would be "inked" by the corresponding drawing operations.
 * 
 * The result of cairo.context_path_extents() is defined as equivalent to the limit of cairo.context_stroke_extents() with cairo.LINE_CAP_ROUND as the line width approaches 0.0, (but never reaching the empty-rectangle returned by cairo.context_stroke_extents() for a line width of 0.0).
 * 
 * Specifically, this means that zero-area sub-paths such as cairo.context_move_to(); cairo.context_line_to() segments, (even degenerate cases where the coordinates to both calls are identical), will be considered as contributing to the extents. However, a lone cairo.context_move_to() will not contribute to the results of cairo.context_path_extents().
 * 
 * The object returned has the following members:
 * 
 * + x1: x coordinate of the top left corner of the resulting extents.
 * + y1: y coordinate of the top left corner of the resulting extents.
 * + x2: x coordinate of the lower right corner of the resulting extents.
 * + y2: y coordinate of the lower right corner of the resulting extents.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} extents - see object description above.
 */
static JSVAL context_path_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x1,y1, x2,y2;
    cairo_path_extents(context, &x1,&y1, &x2,&y2);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    o->Set(String::New("x2"), Number::New(x2));
    o->Set(String::New("y2"), Number::New(y2));
    return o;
}

////////////////////////// TEXT AND GLYPHS

/**
 * @function cairo.context_select_font_face
 * 
 * ### Synopsis
 * 
 * cairo.context_select_font_face(context, family, slant, weight);
 * 
 * Selects a family and style of font from a simplified description as a family name, slant and weight. 
 * 
 * Cairo provides no operation to list available family names on the system (this is a "toy", remember), but the standard CSS2 generic family names, ("serif", "sans-serif", "cursive", "fantasy", "monospace"), are likely to work as expected.
 * 
 * If family starts with the string "cairo:", or if no native font backends are compiled in, cairo will use an internal font family. The internal font family recognizes many modifiers in the family string, most notably, it recognizes the string "monospace". That is, the family name "cairo:monospace" will use the monospace version of the internal font family.
 * 
 * For "real" font selection, see the font-backend-specific font_face_create functions for the font backend you are using. (For example, if you are using the freetype-based cairo-ft font backend, see cairo.ft_font_face_create_for_ft_face() or cairo.ft_font_face_create_for_pattern().) The resulting font face could then be used with cairo.scaled_font_create() and cairo.context_set_scaled_font().
 * 
 * Similarly, when using the "real" font support, you can call directly into the underlying font system, (such as fontconfig or freetype), for operations such as listing available fonts, etc.
 * 
 * It is expected that most applications will need to use a more comprehensive font handling and text layout library, (for example, pango), in conjunction with cairo.
 * 
 * If text is drawn without a call to cairo.context_select_font_face(), (nor cairo.context_set_font_face() nor cairo.context_set_scaled_font()), the default family is platform-specific, but is essentially "sans-serif". Default slant is cairo.FONT_SLANT_NORMAL, and default weight is cairo.FONT_WEIGHT_NORMAL.
 * 
 * This function is equivalent to a call to cairo_toy_font_face_create() followed by cairo.context_set_font_face().
 * 
 * Note: The cairo_select_font_face() function call is part of what the cairo designers call the "toy" text API. It is convenient for short demos and simple programs, but it is not expected to be adequate for serious text-using applications.
 * 
 * The slant argument may be one of:
 * 
 * + cairo.FONT_SLANT_NORMAL - Upright font style
 * + cairo.FONT_SLANT_ITALIC - Italic font style
 * + cairo.FONT_SLANT_OBLIQUE - Oblique font style
 * 
 * The weight argument may be one of:
 * 
 * + cairo.FONT_WEIGHT_NORMAL - Normal font weight.
 * + cairo.FONT_WEIGHT_BOLD - Bold font weight.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {string} family - font family name.
 * @param {int} slant - the slant for the font.
 * @param {int} weight - the font weight for the font.
 */
static JSVAL context_select_font_face(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    String::Utf8Value family(args[1]->ToString());
    cairo_select_font_face(context, *family, (cairo_font_slant_t)args[2]->IntegerValue(), (cairo_font_weight_t)args[3]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.context_set_font_size
 * 
 * ### Synopsis
 * 
 * cairo.context_set_font_size(context, size);
 * 
 * Sets the current font matrix to a scale by a factor of size, replacing any font matrix previously set with cairo.context_set_font_size() or cairo.context_set_font_matrix(). 
 * 
 * This results in a font size of size user space units. (More precisely, this matrix will result in the font's em-square being a size by size square in user space.)
 * 
 * If text is drawn without a call to cairo.context_set_font_size(), (nor cairo.context_set_font_matrix() nor cairo.context_set_scaled_font()), the default font size is 10.0.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} size - the new font size, in user space units.
 */
static JSVAL context_set_font_size(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_font_size(context, args[1]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_set_font_matrix
 * 
 * ### Synopsis
 * 
 * cairo.context_set_font_matrix(context, matrix);
 * 
 * Sets the current font matrix to matrix. The font matrix gives a transformation from the design space of the font (in this space, the em-square is 1 unit by 1 unit) to user space. 
 * 
 * Normally, a simple scale is used (see cairo_set_font_size()), but a more complex font matrix can be used to shear the font or stretch it unequally along the two axes.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} matrix - opaque handle to a cairo matrix.
 */
static JSVAL context_set_font_matrix(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[1]);
    cairo_set_font_matrix(context, matrix);
    return Undefined();
}

/**
 * @function cairo.context_get_font_matrix
 * 
 * ### Synopsis
 * 
 * var matrix = cairo.context_get_font_matrix(context);
 * 
 * Gets the current font matrix.
 * 
 * The matrix returned is owned by the caller and must be released by calling cairo.matrix_destroy() when it is no longer needed.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} matrix - opaque handle to a cairo matrix.
 */
static JSVAL context_get_font_matrix(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_matrix_t *matrix = new cairo_matrix_t;
    cairo_get_font_matrix(context, matrix);
    return External::New(matrix);
}

/**
 * @function cairo.context_set_font_options
 * 
 * ### Synopsis
 * 
 * cairo.context_set_font_options(context, options);
 * 
 * Sets a set of custom font rendering options for the context. 
 * 
 * Rendering options are derived by merging these options with the options derived from underlying surface; if the value in options has a default value (like cairo.ANTIALIAS_DEFAULT), then the value from the surface is used.
 * 
 * See font_options methods for creating the font options object.
 * 
 * The options object is an opaque structure holding all options that are used when rendering fonts.
 * 
 * Individual features of a cairo font_options object can be set or accessed using functions named cairo.font_options_set_feature_name() and cairo.font_options_get_feature_name(), like cairo.font_options_set_antialias() and cairo.font_options_get_antialias().
 * 
 * New features may be added to a cairo font_options object in the future. For this reason, cairo.font_options_copy(), cairo.font_options_equal(), cairo.font_options_merge(), and cairo.font_options_hash() should be used to copy, check for equality, merge, or compute a hash value of cairo font_options objects. 
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} options - opaque handle to a cairo font options object.
 */
static JSVAL context_set_font_options(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[1]);
    cairo_set_font_options(context, options);
    return Undefined();
}

/**
 * @function cairo.context_get_font_options
 * 
 * ### Synopsis
 * 
 * var options = cairo.context_get_font_options(context);
 * 
 * Retrieves font rendering options set via cairo.context_set_font_options. 
 * 
 * Note that the returned options do not include any options derived from the underlying surface; they are literally the options passed to cairo.context_set_font_options().
 * 
 * The caller owns the returned opaque handle to the font options.  It must be freed by calling cairo.font_options_destroy().
 * 
 * The options object is an opaque structure holding all options that are used when rendering fonts.
 * 
 * Individual features of a cairo font_options object can be set or accessed using functions named cairo.font_options_set_feature_name() and cairo.font_options_get_feature_name(), like cairo.font_options_set_antialias() and cairo.font_options_get_antialias().
 * 
 * New features may be added to a cairo font_options object in the future. For this reason, cairo.font_options_copy(), cairo.font_options_equal(), cairo.font_options_merge(), and cairo.font_options_hash() should be used to copy, check for equality, merge, or compute a hash value of cairo font_options objects. 
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} options - opaque handle to a cairo font options object.
 */
static JSVAL context_get_font_options(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_font_options_t *options = cairo_font_options_create();
    cairo_get_font_options(context, options);
    return External::New(options);
}

/**
 * @function cairo.context_set_font_face
 * 
 * ### Synopsis
 * 
 * cairo.context_set_font_face(context, face);
 * 
 * Replaces the current cairo_font_face_t object in the cairo_t with font_face. 
 * 
 * The replaced font face in the context will be destroyed if there are no other references to it.
 * 
 * A cairo font_face specifies all aspects of a font other than the size or font matrix (a font matrix is used to distort a font by shearing it or scaling it unequally in the two directions) . A font face can be set on a context by using cairo.context_set_font_face(); the size and font matrix are set with cairo.context_set_font_size() and cairo.context_set_font_matrix().
 * 
 * There are various types of font faces, depending on the font backend they use. The type of a font face can be queried using cairo.font_face_get_type().
 * 
 * Memory management of cairo font_faces is done with cairo.font_face_reference() and cairo.font_face_destroy().
 * 
 * See also the font_face methods.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} face - opaque handle to a cairo font face object.
 */
static JSVAL context_set_font_face(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_font_face_t *face = (cairo_font_face_t *)JSEXTERN(args[1]);
    cairo_set_font_face(context, face);
    return Undefined();
}

/**
 * @function cairo.context_get_font_face
 * 
 * ### Synopsis
 * 
 * var font_face = cairo.context_get_font_face(context);
 * 
 * Gets the current font face for a context.
 * 
 * The returned object is owned by cairo. To keep a reference to it, you must call cairo.font_face_reference(). 
 * 
 * This function never returns NULL. 
 * 
 * If memory cannot be allocated, a special "nil" font face object will be returned on which cairo.font_face_status() returns cairo.STATUS_NO_MEMORY. 
 * 
 * Using this nil object will cause its error state to propagate to other objects it is passed to, (for example, calling cairo.context_set_font_face() with a nil font will trigger an error that will shutdown the context object).
 * 
 * A cairo font_face specifies all aspects of a font other than the size or font matrix (a font matrix is used to distort a font by shearing it or scaling it unequally in the two directions) . A font face can be set on a context by using cairo.context_set_font_face(); the size and font matrix are set with cairo.context_set_font_size() and cairo.context_set_font_matrix().
 * 
 * There are various types of font faces, depending on the font backend they use. The type of a font face can be queried using cairo.font_face_get_type().
 * 
 * Memory management of cairo font_faces is done with cairo.font_face_reference() and cairo.font_face_destroy().
 * 
 * See also the font_face methods.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} font_face  - opaque handle to a cairo font face object.
 */
static JSVAL context_get_font_face(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_get_font_face(context));
}

/**
 * @function cairo.context_set_scaled_font
 * 
 * ### Synopsis
 * 
 * cairo.context_set_scaled_font(context, scaled_font);
 * 
 * Replaces the current font face, font matrix, and font options in the context with those of the scaled_font. 
 * 
 * Except for some translation, the current CTM of the context should be the same as that of the scaled_font, which can be accessed using cairo.scaled_font_get_ctm().
 * 
 * A cairo scaled_font is a font scaled to a particular size and device resolution. A cairo scaled_font is most useful for low-level font usage where a library or application wants to cache a reference to a scaled font to speed up the computation of metrics.
 * 
 * There are various types of scaled fonts, depending on the font backend they use. The type of a scaled font can be queried using cairo.scaled_font_get_type().
 * 
 * Memory management of cairo scaled_font is done with cairo.scaled_font_reference() and cairo.scaled_font_destroy().
 * 
 * See also the scaled_font methods.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} scaled_font - opaque handle to a cairo scaled font object.
 */
static JSVAL context_set_scaled_font(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[1]);
    cairo_set_scaled_font(context, scaled_font);
    return Undefined();
}

/**
 * @function cairo.context_get_scaled_font
 * 
 * ### Synopsis
 * 
 * var scaled_font = cairo.context_get_scaled_font(context);
 * 
 * Gets the current scaled font for a context.
 * 
 * The returned object is owned by cairo. To keep a reference to it, you must call cairo.scaled_font_reference(). 
 * 
 * This function never returns NULL. 
 * 
 * If memory cannot be allocated, a special "nil" scaled font object will be returned on which cairo.scaled_font_status() returns cairo.STATUS_NO_MEMORY. 
 * 
 * Using this nil object will cause its error state to propagate to other objects it is passed to, (for example, calling cairo.context_set_scaled_font() with a nil font will trigger an error that will shutdown the context object).
 * 
 * A cairo scaled_font is a font scaled to a particular size and device resolution. A cairo scaled_font is most useful for low-level font usage where a library or application wants to cache a reference to a scaled font to speed up the computation of metrics.
 * 
 * There are various types of scaled fonts, depending on the font backend they use. The type of a scaled font can be queried using cairo.scaled_font_get_type().
 * 
 * Memory management of cairo scaled_font is done with cairo.scaled_font_reference() and cairo.scaled_font_destroy().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} scaled_font  - opaque handle to a cairo scaled font object.
 */
static JSVAL context_get_scaled_font(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_get_scaled_font(context));
}

/**
 * @function cairo.context_show_text
 * 
 * ### Synopsis
 * 
 * cairo.context_show_text(context, text);
 * 
 * A drawing operator that generates the shape from a string of UTF-8 characters, rendered according to the current font_face, font_size (font_matrix), and font_options.
 * 
 * This function first computes a set of glyphs for the string of text. The first glyph is placed so that its origin is at the current point. The origin of each subsequent glyph is offset from that of the previous glyph by the advance values of the previous glyph.
 * 
 * After this call the current point is moved to the origin of where the next glyph would be placed in this same progression. That is, the current point will be at the origin of the final glyph offset by its advance values. This allows for easy display of a single logical string with multiple calls to cairo.context_show_text().
 * 
 * Note: The cairo.context_show_text() function call is part of what the cairo designers call the "toy" text API. It is convenient for short demos and simple programs, but it is not expected to be adequate for serious text-using applications. See cairo.context_show_glyphs() for the "real" text display API in cairo.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {string} text - text to render.
 */
static JSVAL context_show_text(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    String::Utf8Value text(args[1]->ToString());
    cairo_show_text(context, *text);
    return Undefined();
}

/**
 * @function cairo.context_show_glyphs
 * 
 * ### Synopsis
 * 
 * cairo.context_show_glyphs(context, glyphs);
 * 
 * A drawing operator that generates the shape from an array of glyphs, rendered according to the current font face, font size (font matrix), and font options.
 * 
 * The glyphs argument is an array of objects of the following form:
 * 
 * + {int} index - glyph index in the font.  The exact interpretation of the glyph index depends on the font technology being used.
 * + {number} x - the offset in the x direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * + {number} y - the offset in the y direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * 
 * The above structure holds information about a single glyph when drawing or measuring text. A font is (in simple terms) a collection of shapes used to draw text. A glyph is one of these shapes. There can be multiple glyphs for a single character (alternates to be used in different contexts, for example), or a glyph can be a ligature of multiple characters. Cairo doesn't expose any way of converting input text into glyphs, so in order to use the Cairo interfaces that take arrays of glyphs, you must directly access the appropriate underlying font system.
 * 
 * Note that the offsets given by x and y are not cumulative. When drawing or measuring text, each glyph is individually positioned with respect to the overall origin
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {array} glyphs - array of objects as described above.
 */
static JSVAL context_show_glyphs(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Handle<Array>glyphs = Handle<Array>::Cast(args[1]->ToObject());
    int num_glyphs = glyphs->Length();
    cairo_glyph_t *c_glyphs = new cairo_glyph_t[num_glyphs];
    // 
    Local<String>index = String::New("index");
    Local<String>x = String::New("x");
    Local<String>y = String::New("y");
    
    for (int i=0; i<num_glyphs; i++) {
        JSOBJ o = glyphs->Get(i)->ToObject();
        c_glyphs[i].index = o->Get(index)->IntegerValue();
        c_glyphs[i].x = o->Get(x)->NumberValue();
        c_glyphs[i].y = o->Get(y)->NumberValue();
    }
    cairo_show_glyphs(context, c_glyphs, num_glyphs);
    delete c_glyphs;
    return Undefined();
}

/**
 * @function cairo.context_show_text_glyphs
 * 
 * ### Synopsis
 * 
 * cairo.context_show_text_glyphs(context, text, glyphs, clusters, cluster_flags);
 * 
 * This operation has rendering effects similar to cairo.context_show_glyphs() but, if the target surface supports it, uses the provided text and cluster mapping to embed the text for the glyphs shown in the output. 
 * 
 * If the target does not support the extended attributes, this function acts like the basic cairo.context_show_glyphs() as if it had been passed glyphs.
 * 
 * The mapping between text and glyphs is provided by an array of clusters. Each cluster covers a number of text bytes and glyphs, and neighboring clusters cover neighboring areas of text and glyphs. The clusters should collectively cover text and glyphs in entirety.
 * 
 * The first cluster always covers bytes from the beginning of text. If cluster_flags do not have the cairo.TEXT_CLUSTER_FLAG_BACKWARD set, the first cluster also covers the beginning of glyphs, otherwise it covers the end of the glyphs array and following clusters move backward.
 * 
 * The glyphs argument is an array of objects of the following form:
 * 
 * + {int} index - glyph index in the font.  The exact interpretation of the glyph index depends on the font technology being used.
 * + {number} x - the offset in the x direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * + {number} y - the offset in the y direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * 
 * The above structure holds information about a single glyph when drawing or measuring text. A font is (in simple terms) a collection of shapes used to draw text. A glyph is one of these shapes. There can be multiple glyphs for a single character (alternates to be used in different contexts, for example), or a glyph can be a ligature of multiple characters. Cairo doesn't expose any way of converting input text into glyphs, so in order to use the Cairo interfaces that take arrays of glyphs, you must directly access the appropriate underlying font system.
 * 
 * Note that the offsets given by x and y are not cumulative. When drawing or measuring text, each glyph is individually positioned with respect to the overall origin
 * 
 * The clusters argument is an array of objects of the following form:
 * + {int} num_bytes - the number of bytes of UTF-8 text covered by this cluster.
 * + {int} num_glyphs - the number of glyphs covered by this cluster.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {string} text - a string of text encoded in UTF-8.
 * @param {array} glyphs - array of glyph objects, described above.
 * @param {array} clusters - array of objects, cluster mapping information as described above.
 * @param {int} cluster_flags - currently 0 or cairo.TEXT_CLUSTER_FLAG_BACKWARD.
 */
static JSVAL context_show_text_glyphs(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    String::Utf8Value text(args[1]->ToString());
    Handle<Array>glyphs = Handle<Array>::Cast(args[2]->ToObject());
    int num_glyphs = glyphs->Length();
    cairo_glyph_t *c_glyphs = new cairo_glyph_t[num_glyphs];
    // 
    Local<String>index = String::New("index");
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    
    for (int i=0; i<num_glyphs; i++) {
        JSOBJ o = glyphs->Get(i)->ToObject();
        c_glyphs[i].index = o->Get(index)->IntegerValue();
        c_glyphs[i].x = o->Get(_x)->NumberValue();
        c_glyphs[i].y = o->Get(_y)->NumberValue();
    }
    
    Handle<Array>clusters = Handle<Array>::Cast(args[3]->ToObject());
    int num_clusters = clusters->Length();
    cairo_text_cluster_t *c_clusters = new cairo_text_cluster_t[num_clusters];
    Local<String>_num_bytes = String::New("num_bytes");
    Local<String>_num_glyphs = String::New("num_glyphs");
    for (int i=0; i<num_clusters; i++) {
        JSOBJ o = clusters->Get(i)->ToObject();
        c_clusters[i].num_bytes = o->Get(_num_bytes)->NumberValue();
        c_clusters[i].num_glyphs = o->Get(_num_glyphs)->NumberValue();
    }
    cairo_show_text_glyphs(context, *text, -1, c_glyphs, num_glyphs, c_clusters, num_clusters, (cairo_text_cluster_flags_t)args[4]->IntegerValue());
    delete c_clusters;
    delete c_glyphs;
    return Undefined();
}

/**
 * @function cairo.context_font_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_font_extents(context);
 * 
 * Gets the font extents for the currently selected font.
 * 
 * The object returned has the following members:
 * 
 * + {number} ascent - the distance that the font extends above the baseline. Note that this is not always exactly equal to the maximum of the extents of all the glyphs in the font, but rather is picked to express the font designer's intent as to how the font should align with elements above it.
 * + {number} descent - the distance that the font extends below the baseline. This value is positive for typical fonts that include portions below the baseline. Note that this is not always exactly equal to the maximum of the extents of all the glyphs in the font, but rather is picked to express the font designer's intent as to how the font should align with elements below it.
 * + {number} height - the recommended vertical distance between baselines when setting consecutive lines of text with the font. This is greater than ascent+descent by a quantity known as the line spacing or external leading. When space is at a premium, most fonts can be set with only a distance of ascent+descent between lines.
 * + {number} max_x_advance - the maximum distance in the X direction that the origin is advanced for any glyph in the font.
 * + {number} max_y_advance - the maximum distance in the Y direction that the origin is advanced for any glyph in the font. This will be zero for normal fonts used for horizontal writing. (The scripts of East Asia are sometimes written vertically.)
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} extents - object as described above.
 */
static JSVAL context_font_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_font_extents_t extents;
    cairo_font_extents(context, &extents);
    JSOBJ o = Object::New();
    o->Set(String::New("ascent"), Number::New(extents.ascent));
    o->Set(String::New("descent"), Number::New(extents.descent));
    o->Set(String::New("height"), Number::New(extents.height));
    o->Set(String::New("max_x_advance"), Number::New(extents.max_x_advance));
    o->Set(String::New("max_y_advance"), Number::New(extents.max_y_advance));
    return o;
}

/**
 * @function cairo.context_text_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_text_extents(context, text);
 * 
 * Gets the extents for a string of text. The extents describe a user-space rectangle that encloses the "inked" portion of the text, (as it would be drawn by cairo.context_show_text()). 
 * 
 * Additionally, the x_advance and y_advance values indicate the amount by which the current point would be advanced by cairo.context_show_text().
 * 
 * Note that whitespace characters do not directly contribute to the size of the rectangle (extents.width and extents.height). They do contribute indirectly by changing the position of non-whitespace characters. In particular, trailing whitespace characters are likely to not affect the size of the rectangle, though they will affect the x_advance and y_advance values.
 * 
 * The object returned has the following members:
 * 
 * + {number} x_bearing - the horizontal distance from the origin to the leftmost part of the glyphs as drawn. Positive if the glyphs lie entirely to the right of the origin.
 * + {number} y_bearing - the vertical distance from the origin to the topmost part of the glyphs as drawn. Positive only if the glyphs lie completely below the origin; will usually be negative.
 * + {number} width - width of the glyphs as drawn.
 * + {number} height - height of the glyphs as drawn.
 * + {number} x_advance - distance to advance in the X direction after drawing these glyphs.
 * + {number} y_advance - distance to advance in the Y direction after drawing these glyphs. Will typically be zero except for vertical text layout as found in East-Asian languages.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {string} text - a strng of text encoded in UTF8.
 * @return {object} extents - object as described above.
 */
static JSVAL context_text_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    String::Utf8Value text(args[1]->ToString());
    cairo_text_extents_t extents;
    cairo_text_extents(context, *text, &extents);
    JSOBJ o = Object::New();
    o->Set(String::New("x_bearing"), Number::New(extents.x_bearing));
    o->Set(String::New("y_bearing"), Number::New(extents.y_bearing));
    o->Set(String::New("width"), Number::New(extents.width));
    o->Set(String::New("height"), Number::New(extents.height));
    o->Set(String::New("x_advance"), Number::New(extents.x_advance));
    o->Set(String::New("y_advance"), Number::New(extents.y_advance));
    return o;
}

/**
 * @function cairo.context_glyph_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_glyph_extents(context, glyphs);
 * 
 * Gets the extents for an array of glyphs. 
 * 
 * The extents describe a user-space rectangle that encloses the "inked" portion of the glyphs, (as they would be drawn by cairo.context_show_glyphs()). 
 * 
 * Additionally, the x_advance and y_advance values indicate the amount by which the current point would be advanced by cairo.context_show_glyphs().
 * 
 * Note that whitespace glyphs do not contribute to the size of the rectangle (extents.width and extents.height).
 * 
 * The glyphs argument is an array of objects of the following form:
 * 
 * + {int} index - glyph index in the font.  The exact interpretation of the glyph index depends on the font technology being used.
 * + {number} x - the offset in the x direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * + {number} y - the offset in the y direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * 
 * The above structure holds information about a single glyph when drawing or measuring text. A font is (in simple terms) a collection of shapes used to draw text. A glyph is one of these shapes. There can be multiple glyphs for a single character (alternates to be used in different contexts, for example), or a glyph can be a ligature of multiple characters. Cairo doesn't expose any way of converting input text into glyphs, so in order to use the Cairo interfaces that take arrays of glyphs, you must directly access the appropriate underlying font system.
 * 
 * Note that the offsets given by x and y are not cumulative. When drawing or measuring text, each glyph is individually positioned with respect to the overall origin
 * 
 * The object returned has the following members:
 * 
 * + {number} x_bearing - the horizontal distance from the origin to the leftmost part of the glyphs as drawn. Positive if the glyphs lie entirely to the right of the origin.
 * + {number} y_bearing - the vertical distance from the origin to the topmost part of the glyphs as drawn. Positive only if the glyphs lie completely below the origin; will usually be negative.
 * + {number} width - width of the glyphs as drawn.
 * + {number} height - height of the glyphs as drawn.
 * + {number} x_advance - distance to advance in the X direction after drawing these glyphs.
 * + {number} y_advance - distance to advance in the Y direction after drawing these glyphs. Will typically be zero except for vertical text layout as found in East-Asian languages.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {array} glyphs - array of objects as described above.
 * @return {object} extents - object as described above.
 */
static JSVAL context_glyph_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Handle<Array>glyphs = Handle<Array>::Cast(args[2]->ToObject());
    int num_glyphs = glyphs->Length();
    cairo_glyph_t *c_glyphs = new cairo_glyph_t[num_glyphs];
    // 
    Local<String>index = String::New("index");
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    
    for (int i=0; i<num_glyphs; i++) {
        JSOBJ o = glyphs->Get(i)->ToObject();
        c_glyphs[i].index = o->Get(index)->IntegerValue();
        c_glyphs[i].x = o->Get(_x)->NumberValue();
        c_glyphs[i].y = o->Get(_y)->NumberValue();
    }
    
    cairo_text_extents_t extents;
    cairo_glyph_extents(context, c_glyphs, num_glyphs, &extents);
    delete [] c_glyphs;

    JSOBJ o = Object::New();
    o->Set(String::New("x_bearing"), Number::New(extents.x_bearing));
    o->Set(String::New("y_bearing"), Number::New(extents.y_bearing));
    o->Set(String::New("width"), Number::New(extents.width));
    o->Set(String::New("height"), Number::New(extents.height));
    o->Set(String::New("x_advance"), Number::New(extents.x_advance));
    o->Set(String::New("y_advance"), Number::New(extents.y_advance));
    return o;
}

/**
 * @function cairo.toy_font_face_create
 * 
 * ### Synopsis
 * 
 * var font_face = cairo.toy_font_face_create(family, slant, weight);
 * 
 * Creates a font face from a triplet of family, slant, and weight. 
 * 
 * These font faces are used in implementation of the the cairo_t "toy" font API.
 * 
 * If family is the zero-length string "", the platform-specific default family is assumed. The default family then can be queried using cairo.toy_font_face_get_family().
 * 
 * The cairo.context_select_font_face() function uses this to create font faces. See that function for limitations and other details of toy font faces.
 * 
 * The slant argument may be one of:
 * 
 * + cairo.FONT_SLANT_NORMAL - Upright font style
 * + cairo.FONT_SLANT_ITALIC - Italic font style
 * + cairo.FONT_SLANT_OBLIQUE - Oblique font style
 * 
 * The weight argument may be one of:
 * 
 * + cairo.FONT_WEIGHT_NORMAL - Normal font weight.
 * + cairo.FONT_WEIGHT_BOLD - Bold font weight.
 * 
 * A cairo font_face specifies all aspects of a font other than the size or font matrix (a font matrix is used to distort a font by shearing it or scaling it unequally in the two directions) . A font face can be set on a context by using cairo.context_set_font_face(); the size and font matrix are set with cairo.context_set_font_size() and cairo.context_set_font_matrix().
 * 
 * There are various types of font faces, depending on the font backend they use. The type of a font face can be queried using cairo.font_face_get_type().
 * 
 * Memory management of cairo font_faces is done with cairo.font_face_reference() and cairo.font_face_destroy().
 * 
 * See also the font_face methods.
 * 
 * AVAILABLE IN CAIRO 1.8 OR NEWER
 * 
 * @param {string} family - font family name.
 * @param {int} slant - the slant for the font.
 * @param {int} weight - the font weight for the font.
 * @return {object} font_face - opaque handle to a cairo font face object.
 */
#if CAIRO_VERSION_MINOR >= 8
static JSVAL toy_font_face_create(JSARGS args) {
    String::Utf8Value family(args[0]->ToString());
    cairo_font_face_t *font_face = cairo_toy_font_face_create(*family, (cairo_font_slant_t)args[1]->IntegerValue(), (cairo_font_weight_t)args[2]->IntegerValue());
    return External::New(font_face);
}
#endif

/**
 * @function cairo.toy_font_face_get_family
 * 
 * ### Synopsis
 * 
 * var family = cairo.toy_font_face_get_family(font_face);
 * 
 * Gets the family name of a toy font.
 * 
 * A cairo font_face specifies all aspects of a font other than the size or font matrix (a font matrix is used to distort a font by shearing it or scaling it unequally in the two directions) . A font face can be set on a context by using cairo.context_set_font_face(); the size and font matrix are set with cairo.context_set_font_size() and cairo.context_set_font_matrix().
 * 
 * There are various types of font faces, depending on the font backend they use. The type of a font face can be queried using cairo.font_face_get_type().
 * 
 * Memory management of cairo font_faces is done with cairo.font_face_reference() and cairo.font_face_destroy().
 * 
 * See also the font_face methods.
 * 
 * AVAILABLE IN CAIRO 1.8 OR NEWER
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 * @return {string} family - The family name.
 */
#if CAIRO_VERSION_MINOR >= 8
static JSVAL toy_font_face_get_family(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    return String::New(cairo_toy_font_face_get_family(font_face));
}
#endif

/**
 * @function cairo.toy_font_face_get_slant
 * 
 * ### Synopsis
 * 
 * var slant = cairo.toy_font_face_get_slant(font_face);
 * 
 * Gets the slant of a toy font.
 * 
 * The slant returned may be one of:
 * 
 * + cairo.FONT_SLANT_NORMAL - Upright font style
 * + cairo.FONT_SLANT_ITALIC - Italic font style
 * + cairo.FONT_SLANT_OBLIQUE - Oblique font style
 * 
 * A cairo font_face specifies all aspects of a font other than the size or font matrix (a font matrix is used to distort a font by shearing it or scaling it unequally in the two directions) . A font face can be set on a context by using cairo.context_set_font_face(); the size and font matrix are set with cairo.context_set_font_size() and cairo.context_set_font_matrix().
 * 
 * There are various types of font faces, depending on the font backend they use. The type of a font face can be queried using cairo.font_face_get_type().
 * 
 * Memory management of cairo font_faces is done with cairo.font_face_reference() and cairo.font_face_destroy().
 * 
 * See also the font_face methods.
 * 
 * AVAILABLE IN CAIRO 1.8 OR NEWER
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 * @return {int} slant - The family name.
 */
#if CAIRO_VERSION_MINOR >= 8
static JSVAL toy_font_face_get_slant(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_toy_font_face_get_slant(font_face));
}
#endif

/**
 * @function cairo.toy_font_face_get_weight
 * 
 * ### Synopsis
 * 
 * var weight = cairo.toy_font_face_get_weight(font_face);
 * 
 * Gets the slant of a toy font.
 * 
 * The weight returned may be one of:
 * 
 * + cairo.FONT_WEIGHT_NORMAL - Normal font weight.
 * + cairo.FONT_WEIGHT_BOLD - Bold font weight.
 * 
 * A cairo font_face specifies all aspects of a font other than the size or font matrix (a font matrix is used to distort a font by shearing it or scaling it unequally in the two directions) . A font face can be set on a context by using cairo.context_set_font_face(); the size and font matrix are set with cairo.context_set_font_size() and cairo.context_set_font_matrix().
 * 
 * There are various types of font faces, depending on the font backend they use. The type of a font face can be queried using cairo.font_face_get_type().
 * 
 * Memory management of cairo font_faces is done with cairo.font_face_reference() and cairo.font_face_destroy().
 * 
 * See also the font_face methods.
 * 
 * AVAILABLE IN CAIRO 1.8 OR NEWER
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 * @return {int} weight - font weight.
 */
#if CAIRO_VERSION_MINOR >= 8
static JSVAL toy_font_face_get_weight(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_toy_font_face_get_weight(font_face));
}
#endif

////////////////////////// FONT FACES

/**
 * @function cairo.font_face_reference
 * 
 * ### Synopsis
 * 
 * var font_face = cairo.font_face_reference(font_face);
 * 
 * Increases the reference count on font_face by one. 
 * 
 * This prevents font_face from being destroyed until a matching call to cairo.font_face_destroy() is made.
 * 
 * The number of references to a cairo font_face can be get using cairo.font_face_get_reference_count().
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 * @return {object} font_face - opaque handle to the referenced cairo font face object.
 */
static JSVAL font_face_reference(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    return External::New(cairo_font_face_reference(font_face));
}

/**
 * @function cairo.font_face_destroy
 * 
 * ### Synopsis
 * 
 * cairo.font_face_destroy(font_face);
 * 
 * Decreases the reference count on font_face by one. 
 * 
 * If the result is zero, then font_face and all associated resources are freed. See cairo.font_face_reference().
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 */
static JSVAL font_face_destroy(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    cairo_font_face_destroy(font_face);
    return Undefined();
}

/**
 * @function cairo.font_face_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.font_face_status(font_face);
 * 
 * Checks whether an error has previously occurred for this font face
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 * @return {int} status - cairo.STATUS_SUCCESS or another error such as cairo.STATUS_NO_MEMORY.
 */
static JSVAL font_face_status(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_face_status(font_face));
}

/**
 * @function cairo.font_face_get_type
 * 
 * ### Synopsis
 * 
 * var type = cairo.font_face_get_type(font_face);
 * 
 * This function returns the type of the backend used to create a font face.
 * 
 * The font types are also known as "font backends" within cairo.
 * 
 * The type of a font face is determined by the function used to create it, which will generally be of the form cairo.type_font_face_create(). The font face type can be queried with cairo.font_face_get_type()
 * 
 * The various cairo font_face functions can be used with a font face of any type.
 * 
 * The type of a scaled font is determined by the type of the font face passed to cairo.scaled_font_create(). The scaled font type can be queried with cairo.scaled_font_get_type().
 * 
 * The various cairo scaled_font functions can be used with scaled fonts of any type, but some font backends also provide type-specific functions that must only be called with a scaled font of the appropriate type. These functions have names that begin with cairo.type_scaled_font() such as cairo.ft_scaled_font_lock_face().
 * 
 * The behavior of calling a type-specific function with a scaled font of the wrong type is undefined.
 * 
 * New entries may be added in future versions.
 * 
 * cairo.FONT_TYPE_TOY - The font was created using cairo's toy font API (Since: 1.2).
 * cairo.FONT_TYPE_FT - The font is of type FreeType (Since: 1.2).
 * cairo.FONT_TYPE_WIN32 - The font is of type Win32 (Since: 1.2).
 * cairo.FONT_TYPE_QUARTZ - The font is of type Quartz (Since: 1.6, in 1.2 and 1.4 it was named cairo.FONT_TYPE_ATSUI).
 * cairo.FONT_TYPE_ATSUI - The font is of type Quartz (since 1.2, backward compatibility for cairo.FONT_TYPE_ATSUI).
 * cairo.FONT_TYPE_USER - The font was create using cairo's user font API (Since: 1.8).
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 * @return {int} type - one of the above types.
 */
static JSVAL font_face_get_type(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_face_get_type(font_face));
}

/**
 * @function cairo.font_face_get_reference_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.font_face_get_reference_count(font_face);
 * 
 * Get the current reference count of font_face.
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 * @return {int} count - current reference count of font_face.  If a nil font_face, 0 will be returned.
 */
static JSVAL font_face_get_reference_count(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_face_get_reference_count(font_face));
}

////////////////////////// SCALED FONTS

/**
 * @function cairo.scaled_font_create
 * 
 * ### Synopsis
 * 
 * var scaled_font = cairo.scaled_font_create(font_face, font_matrix, ctm, options);
 * 
 * Create a cairo scaled_font object from a font_face and matrices that describe the size of the font and the environment in which it will be used.
 * 
 * A cairo scaled_font is a font scaled to a particular size and device resolution. A cairo scaled_font is most useful for low-level font usage where a library or application wants to cache a reference to a scaled font to speed up the computation of metrics.
 * 
 * There are various types of scaled fonts, depending on the font backend they use. The type of a scaled font can be queried using cairo.scaled_font_get_type().
 * 
 * Memory management of cairo scaled_font is done with cairo.scaled_font_reference() and cairo.scaled_font_destroy().
 * 
 * The font_matrix argument is a font space to user space transformation matrix for the font. In the simplest case of a N point font, this matrix is just a scale by N, but it can also be used to shear the font or stretch it unequally along the two axes. See cairo.context_set_font_matrix().
 * 
 * The options object is an opaque structure holding all options that are used when rendering fonts.
 * 
 * Individual features of a cairo font_options object can be set or accessed using functions named cairo.font_options_set_feature_name() and cairo.font_options_get_feature_name(), like cairo.font_options_set_antialias() and cairo.font_options_get_antialias().
 * 
 * New features may be added to a cairo font_options object in the future. For this reason, cairo.font_options_copy(), cairo.font_options_equal(), cairo.font_options_merge(), and cairo.font_options_hash() should be used to copy, check for equality, merge, or compute a hash value of cairo font_options objects. 
 * 
 * @param {object} font_face - opaque handle to a cairo font face object.
 * @param {object} font_matrix - opaque handle to a cairo matrix, described above.
 * @param {object} ctm - opaque handle to a cairo matrix; user to device transformation matrix with which the font will be used.
 * @param {object} options - opaque handle to a cairo font options object.
 * @return {object} scaled_font - opaque handle to a scaled font.
 */
static JSVAL scaled_font_create(JSARGS args) {
    cairo_font_face_t *font_face = (cairo_font_face_t *)JSEXTERN(args[0]);
    cairo_matrix_t *font_matrix = (cairo_matrix_t *) JSEXTERN(args[1]);
    cairo_matrix_t *ctm = (cairo_matrix_t *) JSEXTERN(args[2]);
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[3]);
    return External::New(cairo_scaled_font_create(font_face, font_matrix, ctm, options));
}

/**
 * @function cairo.scaled_font_reference
 * 
 * ### Synopsis
 * 
 * cairo.scaled_font_reference(scaled_font);
 * 
 * Increases the reference count on scaled_font by one. 
 * 
 * This prevents scaled_font from being destroyed until a matching call to cairo.scaled_font_destroy() is made.
 * 
 * The number of references to a scaled_font can be get using cairo.scaled_font_get_reference_count().
 * 
 * @param {object} scaled_font - opaque handle to a scaled font.
 * @return {object} scaled_font - opaque handle to referenced scaled font.
 */
static JSVAL scaled_font_reference(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    return External::New(cairo_scaled_font_reference(scaled_font));
}

/**
 * @function cairo.scaled_font_destroy
 * 
 * ### Synopsis
 * 
 * cairo.scaled_font_destroy(scaled_font);
 * 
 * Decreases the reference count on scaled_font by one. 
 * 
 * If the result is zero, then scaled_font and all associated resources are freed. See cairo.scaled_font_reference().
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font object.
 */
static JSVAL scaled_font_destroy(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    cairo_scaled_font_destroy(scaled_font);
    return Undefined();
}

/**
 * @function cairo.scaled_font_get_reference_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.scaled_font_get_reference_count(scaled_font);
 * 
 * Get the current reference count of scaled_font.
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font object.
 * @return {int} count - current reference count of scaled_font.  If a nil scaled_font, 0 will be returned.
 */
#if CAIRO_VERSION_MINOR >= 4
static JSVAL scaled_font_get_reference_count(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_scaled_font_get_reference_count(scaled_font));
}
#endif

/**
 * @function cairo.scaled_font_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.scaled_font_status(scaled_font);
 * 
 * Checks whether an error has previously occurred for this scaled_font.
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font object.
 * @return {int} status - cairo.STATUS_SUCCESS or another error such as cairo.STATUS_NO_MEMORY.
 */
static JSVAL scaled_font_status(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_scaled_font_status(scaled_font));
}

/**
 * @function cairo.scaled_font_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.scaled_font_extents(scaled_font);
 * 
 * Gets the font extents for scaled_font.
 * 
 * The object returned has the following members:
 * 
 * + {number} ascent - the distance that the font extends above the baseline. Note that this is not always exactly equal to the maximum of the extents of all the glyphs in the font, but rather is picked to express the font designer's intent as to how the font should align with elements above it.
 * + {number} descent - the distance that the font extends below the baseline. This value is positive for typical fonts that include portions below the baseline. Note that this is not always exactly equal to the maximum of the extents of all the glyphs in the font, but rather is picked to express the font designer's intent as to how the font should align with elements below it.
 * + {number} height - the recommended vertical distance between baselines when setting consecutive lines of text with the font. This is greater than ascent+descent by a quantity known as the line spacing or external leading. When space is at a premium, most fonts can be set with only a distance of ascent+descent between lines.
 * + {number} max_x_advance - the maximum distance in the X direction that the origin is advanced for any glyph in the font.
 * + {number} max_y_advance - the maximum distance in the Y direction that the origin is advanced for any glyph in the font. This will be zero for normal fonts used for horizontal writing. (The scripts of East Asia are sometimes written vertically.)
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font object.
 * @return {object} extents - object as described above.
 */
static JSVAL scaled_font_extents(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    cairo_font_extents_t extents;
    cairo_scaled_font_extents(scaled_font, &extents);
    JSOBJ o = Object::New();
    o->Set(String::New("ascent"), Number::New(extents.ascent));
    o->Set(String::New("descent"), Number::New(extents.descent));
    o->Set(String::New("height"), Number::New(extents.height));
    o->Set(String::New("max_x_advance"), Number::New(extents.max_x_advance));
    o->Set(String::New("max_y_advance"), Number::New(extents.max_y_advance));
    return o;
}

/**
 * @function cairo.scaled_font_text_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.scaled_font_text_extents(scaled_font, text);
 * 
 * Gets the extents for a string of text. The extents describe a user-space rectangle that encloses the "inked" portion of the text, (as it would be drawn by cairo.context_show_text()). 
 * 
 * Additionally, the x_advance and y_advance values indicate the amount by which the current point would be advanced by cairo.context_show_text().
 * 
 * Note that whitespace characters do not directly contribute to the size of the rectangle (extents.width and extents.height). They do contribute indirectly by changing the position of non-whitespace characters. In particular, trailing whitespace characters are likely to not affect the size of the rectangle, though they will affect the x_advance and y_advance values.
 * 
 * The object returned has the following members:
 * 
 * + {number} x_bearing - the horizontal distance from the origin to the leftmost part of the glyphs as drawn. Positive if the glyphs lie entirely to the right of the origin.
 * + {number} y_bearing - the vertical distance from the origin to the topmost part of the glyphs as drawn. Positive only if the glyphs lie completely below the origin; will usually be negative.
 * + {number} width - width of the glyphs as drawn.
 * + {number} height - height of the glyphs as drawn.
 * + {number} x_advance - distance to advance in the X direction after drawing these glyphs.
 * + {number} y_advance - distance to advance in the Y direction after drawing these glyphs. Will typically be zero except for vertical text layout as found in East-Asian languages.
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font object.
 * @param {string} text - a stirng of text encoded in UTF8.
 * @return {object} extents - object as described above.
 */
#if CAIRO_VERSION_MINOR >= 2
static JSVAL scaled_font_text_extents(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    String::Utf8Value text(args[1]->ToString());
    cairo_text_extents_t extents;
    cairo_scaled_font_text_extents(scaled_font, *text, &extents);
    JSOBJ o = Object::New();
    o->Set(String::New("x_bearing"), Number::New(extents.x_bearing));
    o->Set(String::New("y_bearing"), Number::New(extents.y_bearing));
    o->Set(String::New("width"), Number::New(extents.width));
    o->Set(String::New("height"), Number::New(extents.height));
    o->Set(String::New("x_advance"), Number::New(extents.x_advance));
    o->Set(String::New("y_advance"), Number::New(extents.y_advance));
    return o;
}
#endif

/**
 * @function cairo.scaled_font_glyph_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.scaled_font_glyph_extents(scaled_font, glyphs);
 * 
 * Gets the extents for an array of glyphs. 
 * 
 * The extents describe a user-space rectangle that encloses the "inked" portion of the glyphs, (as they would be drawn by cairo.context_show_glyphs()). 
 * 
 * Additionally, the x_advance and y_advance values indicate the amount by which the current point would be advanced by cairo.context_show_glyphs().
 * 
 * Note that whitespace glyphs do not contribute to the size of the rectangle (extents.width and extents.height).
 * 
 * The glyphs argument is an array of objects of the following form:
 * 
 * + {int} index - glyph index in the font.  The exact interpretation of the glyph index depends on the font technology being used.
 * + {number} x - the offset in the x direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * + {number} y - the offset in the y direction between the origin used for drawing or measuring the string and the origin of this glyph.
 * 
 * The above structure holds information about a single glyph when drawing or measuring text. A font is (in simple terms) a collection of shapes used to draw text. A glyph is one of these shapes. There can be multiple glyphs for a single character (alternates to be used in different contexts, for example), or a glyph can be a ligature of multiple characters. Cairo doesn't expose any way of converting input text into glyphs, so in order to use the Cairo interfaces that take arrays of glyphs, you must directly access the appropriate underlying font system.
 * 
 * Note that the offsets given by x and y are not cumulative. When drawing or measuring text, each glyph is individually positioned with respect to the overall origin
 * 
 * The object returned has the following members:
 * 
 * + {number} x_bearing - the horizontal distance from the origin to the leftmost part of the glyphs as drawn. Positive if the glyphs lie entirely to the right of the origin.
 * + {number} y_bearing - the vertical distance from the origin to the topmost part of the glyphs as drawn. Positive only if the glyphs lie completely below the origin; will usually be negative.
 * + {number} width - width of the glyphs as drawn.
 * + {number} height - height of the glyphs as drawn.
 * + {number} x_advance - distance to advance in the X direction after drawing these glyphs.
 * + {number} y_advance - distance to advance in the Y direction after drawing these glyphs. Will typically be zero except for vertical text layout as found in East-Asian languages.
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font.
 * @param {array} glyphs - array of objects as described above.
 * @return {object} extents - object as described above.
 */
static JSVAL scaled_font_glyph_extents(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    Handle<Array>glyphs = Handle<Array>::Cast(args[2]->ToObject());
    int num_glyphs = glyphs->Length();
    cairo_glyph_t *c_glyphs = new cairo_glyph_t[num_glyphs];
    // 
    Local<String>index = String::New("index");
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    
    for (int i=0; i<num_glyphs; i++) {
        JSOBJ o = glyphs->Get(i)->ToObject();
        c_glyphs[i].index = o->Get(index)->IntegerValue();
        c_glyphs[i].x = o->Get(_x)->NumberValue();
        c_glyphs[i].y = o->Get(_y)->NumberValue();
    }
    
    cairo_text_extents_t extents;
    cairo_scaled_font_glyph_extents(scaled_font, c_glyphs, num_glyphs, &extents);
    delete [] c_glyphs;

    JSOBJ o = Object::New();
    o->Set(String::New("x_bearing"), Number::New(extents.x_bearing));
    o->Set(String::New("y_bearing"), Number::New(extents.y_bearing));
    o->Set(String::New("width"), Number::New(extents.width));
    o->Set(String::New("height"), Number::New(extents.height));
    o->Set(String::New("x_advance"), Number::New(extents.x_advance));
    o->Set(String::New("y_advance"), Number::New(extents.y_advance));
    return o;
}

/**
 * @function cairo.scaled_font_get_font_face
 * 
 * ### Synopsis
 * 
 * var font_face = cairo.scaled_font_get_font_face(scaled_font);
 * 
 * Gets the font face that this scaled font uses. 
 * 
 * This might be the font face passed to cairo.scaled_font_create(), but this does not hold true for all possible cases.
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font.
 * @return {object} font_face  - opaque handle to a cairo font face object.
 */
#if CAIRO_VERSION_MINOR >= 2
static JSVAL scaled_font_get_font_face(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    return External::New(cairo_scaled_font_get_font_face(scaled_font));
}
#endif

/**
 * @function cairo.scaled_font_get_font_options
 * 
 * ### Synopsis
 * 
 * var options = cairo.scaled_font_get_font_options(context);
 * 
 * Get the font options with which scaled_font was created.
 * 
 * NOTE: the resulting options object should be freed with cairo.font_options_destroy().
 * 
 * The options object returned is an opaque structure holding all options that are used when rendering fonts.
 * 
 * Individual features of a cairo font_options object can be set or accessed using functions named cairo.font_options_set_feature_name() and cairo.font_options_get_feature_name(), like cairo.font_options_set_antialias() and cairo.font_options_get_antialias().
 * 
 * New features may be added to a cairo font_options object in the future. For this reason, cairo.font_options_copy(), cairo.font_options_equal(), cairo.font_options_merge(), and cairo.font_options_hash() should be used to copy, check for equality, merge, or compute a hash value of cairo font_options objects. 
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font.
 * @return {object} options - opaque handle to a cairo font options object.
 */
#if CAIRO_VERSION_MINOR >= 2
static JSVAL scaled_font_get_font_options(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    cairo_font_options_t *options = cairo_font_options_create();
    cairo_scaled_font_get_font_options(scaled_font, options);
    return External::New(options);
}
#endif

/**
 * @function cairo.scaled_font_get_font_matrix
 * 
 * ### Synopsis
 * 
 * var matrix = cairo.scaled_font_get_font_matrix(scaled_font);
 * 
 * Gets the font matrix with which scaled_font was created.
 * 
 * The matrix returned is owned by the caller and must be released by calling cairo.matrix_destroy() when it is no longer needed.
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font.
 * @return {object} matrix - opaque handle to a cairo matrix.
 */
#if CAIRO_VERSION_MINOR >= 2
static JSVAL scaled_font_get_font_matrix(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    cairo_matrix_t *matrix = new cairo_matrix_t;
    cairo_scaled_font_get_font_matrix(scaled_font, matrix);
    return External::New(matrix);
}
#endif

/**
 * @function cairo.scaled_font_get_ctm
 * 
 * ### Synopsis
 * 
 * var matrix = cairo.scaled_font_get_ctm(scaled_font);
 * 
 * Gets the CTM with which scaled_font was created. 
 * 
 * Note that the translation offsets (x0, y0) of the CTM are ignored by cairo.scaled_font_create(). So, the matrix this function returns always has 0,0 as x0,y0.
 * 
 * The matrix returned is owned by the caller and must be released by calling cairo.matrix_destroy() when it is no longer needed.
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font.
 * @return {object} matrix - opaque handle to a cairo matrix.
 */
#if CAIRO_VERSION_MINOR >= 2
static JSVAL scaled_font_get_ctm(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    cairo_matrix_t *matrix = new cairo_matrix_t;
    cairo_scaled_font_get_ctm(scaled_font, matrix);
    return External::New(matrix);
}
#endif

/**
 * @function cairo.scaled_font_get_scale_matrix
 * 
 * ### Synopsis
 * 
 * var matrix = cairo.scaled_font_get_scale_matrix(scaled_font);
 * 
 * Stores the scale matrix of scaled_font into matrix. 
 * 
 * The scale matrix is product of the font matrix and the ctm associated with the scaled font, and hence is the matrix mapping from font space to device space.
 * 
 * The matrix returned is owned by the caller and must be released by calling cairo.matrix_destroy() when it is no longer needed.
 * 
 * AVAILABLE IN CAIRO 1.8 OR NEWER
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font.
 * @return {object} matrix - opaque handle to a cairo matrix.
 */
#if CAIRO_VERSION_MINOR >= 8
static JSVAL scaled_font_get_scale_matrix(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    cairo_matrix_t *matrix = new cairo_matrix_t;
    cairo_scaled_font_get_scale_matrix(scaled_font, matrix);
    return External::New(matrix);
}
#endif

/**
 * @function cairo.scaled_font_get_type
 * 
 * ### Synopsis
 * 
 * var type = cairo.scaled_font_get_type(scaled_font);
 * 
 * This function returns the type of the backend used to create a scaled font. 
 * 
 * The font types are also known as "font backends" within cairo.
 * 
 * The type of a font face is determined by the function used to create it, which will generally be of the form cairo.type_font_face_create(). The font face type can be queried with cairo.font_face_get_type()
 * 
 * The various cairo font_face functions can be used with a font face of any type.
 * 
 * The type of a scaled font is determined by the type of the font face passed to cairo.scaled_font_create(). The scaled font type can be queried with cairo.scaled_font_get_type().
 * 
 * The various cairo scaled_font functions can be used with scaled fonts of any type, but some font backends also provide type-specific functions that must only be called with a scaled font of the appropriate type. These functions have names that begin with cairo.type_scaled_font() such as cairo.ft_scaled_font_lock_face().
 * 
 * The behavior of calling a type-specific function with a scaled font of the wrong type is undefined.
 * 
 * New entries may be added in future versions.
 * 
 * cairo.FONT_TYPE_FT - The font is of type FreeType (Since: 1.2).
 * cairo.FONT_TYPE_WIN32 - The font is of type Win32 (Since: 1.2).
 * cairo.FONT_TYPE_QUARTZ - The font is of type Quartz (Since: 1.6, in 1.2 and 1.4 it was named cairo.FONT_TYPE_ATSUI).
 * cairo.FONT_TYPE_ATSUI - The font is of type Quartz (since 1.2, backward compatibility for cairo.FONT_TYPE_ATSUI).
 * cairo.FONT_TYPE_USER - The font was create using cairo's user font API (Since: 1.8).
 * 
 * _This function never returns cairo.FONT_TYPE_TOY._
 * 
 * @param {object} scaled_font - opaque handle to a cairo scaled_font object.
 * @return {int} type - one of the above types.
 */
#if CAIRO_VERSION_MINOR >= 2
static JSVAL scaled_font_get_type(JSARGS args) {
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_scaled_font_get_type(scaled_font));
}
#endif



////////////////////////// FONT OPTIONS

/**
 * @function cairo.font_options_create
 * 
 * ### Synopsis
 * 
 * var options = cairo.font_options_create();
 * 
 * Allocates a new font options object with all options initialized to default values.
 * 
 * Free the returned object with cairo.font_options_destroy(). 
 * 
 * This function always returns a valid pointer; if memory cannot be allocated, then a special error object is returned where all operations on the object do nothing. You can check for this with cairo.font_options_status().
 * 
 * @return {object} options - opaque handle to a font options object.
 */
static JSVAL font_options_create(JSARGS args) {
    return External::New(cairo_font_options_create());
}

/**
 * @function cairo.font_options_copy
 * 
 * ### Synopsis
 * 
 * var copy = cairo.font_options_copy(original);
 * 
 * Allocates a new font options object copying the option values from original.
 * 
 * Free the returned object with cairo.font_options_destroy(). 
 * 
 * This function always returns a valid pointer; if memory cannot be allocated, then a special error object is returned where all operations on the object do nothing. You can check for this with cairo.font_options_status().
 * 
 * @param {object} original- opaque handle to a font options object.
 * @return {object} copy - opaque handle to a copy of the original font options object.
 */
static JSVAL font_options_copy(JSARGS args) {
    cairo_font_options_t *original = (cairo_font_options_t *)JSEXTERN(args[0]);
    return External::New(cairo_font_options_copy(original));
}

/**
 * @functino cairo.font_options_destroy
 * 
 * ### Synopsis
 * 
 * cairo.font_options_destroy(options);
 * 
 * Destroys a cairo font_options object created with cairo.font_options_create() or cairo.font_options_copy().
 * 
 * @return {object} options - opaque handle to a font options object.
 * @return 
 */
static JSVAL font_options_destroy(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    cairo_font_options_destroy(options);
    return Undefined();
}

/**
 * @function cairo.font_options_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.font_options_status(options);
 * 
 * Checks whether an error has previously occurred for this font options object
 * 
 * @param {object} options - opaque handle to a font options object.
 * @return {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
static JSVAL font_options_status(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_options_status(options));
}

/**
 * @function cairo.font_options_merge
 * 
 * ### Synopsis
 * 
 * cairo.font_options_merge(options, other);
 * 
 * Merges non-default options from other into options, replacing existing values. 
 * 
 * This operation can be thought of as somewhat similar to compositing other onto options with the operation of cairo.OPERATOR_OVER.
 * 
 * @param {object} options - opaque handle to a font options object.
 * @param {object} other - opaque handle to a font options object.
 */
static JSVAL font_options_merge(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    cairo_font_options_t *other = (cairo_font_options_t *)JSEXTERN(args[1]);
    cairo_font_options_merge(options, other);
    return Undefined();
}

/**
 * @function cairo.font_options_hash
 * 
 * ### Synopsis
 * 
 * var hash = cairo.font_options_hash(options);
 * 
 * Compute a hash for the font options object; this value will be useful when storing an object containing a cairo font_options in a hash table.
 * 
 * @return {object} options - opaque handle to a font options object.
 * @return {int} hash - the hash value for the font options object, cast to a 32-bit value.
 */
static JSVAL font_options_hash(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_options_hash(options));
}

/**
 * @function cairo.font_options_equal
 * 
 * ### Synopsis
 * 
 * var is_equal = cairo.font_options_equal(options, other);
 * 
 * Compare two font options objects for equality.
 * 
 * @param {object} options - opaque handle to a font options object.
 * @param {object} other - opaque handle to a font options object.
 * @return {boolean} is_equal - true if the two font options objects are equal.
 */
static JSVAL font_options_equal(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    cairo_font_options_t *other = (cairo_font_options_t *)JSEXTERN(args[1]);
    return cairo_font_options_equal(options, other) ? True() : False();
}

/**
 * @function cairo.font_options_set_antialias
 * 
 * Synopsis:
 * 
 * cairo.font_options_set_antialias(options, mode);
 * 
 * Sets the antialiasing mode for the font options object. 
 * 
 * This specifies the type of antialiasing to do when rendering text.
 * 
 * Valid modes are:
 * 
 * + cairo.ANTIALIAS_DEFAULT - use the default antialiasing for the subsystem and target device.
 * + cairo.ANTIALIAS_NONE - use a bilevel alpha mask.
 * + cairo.ANTIALIAS_GRAY - perform single-color antialiasing (using shades of gray for black text on a white background, for example).
 * + cairo.ANTIALIAS_SUBPIXEL - perform antialiasing by taking advantage of the order of subpixel elements on devices such as LCD panels.
 * + cairo_ANTIALIAS_FAST - Hint that the backend should perform some antialiasing but prefer speed over quality, since 1.12.
 * + cairo.ANTIALIAS_GOOD - The backend should balance quality against performance, since 1.12.
 * + CAIRO_ANTIALIAS_BEST - Hint that the backend should render at the highest quality, sacrificing speed if necessary, since 1.12
* 
 * @param {object} options - opaque handle to a font options object.
 * @param {int} mode - one of the above modes.
 */
static JSVAL font_options_set_antialias(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    cairo_font_options_set_antialias(options, (cairo_antialias_t)args[1]->IntegerValue());
    return Undefined();
}
/**
 * @function cairo.font_options_get_antialias
 * 
 * Synopsis:
 * 
 * var mode = cairo.font_options_get_antialias(options);
 * 
 * Gets the antialiasing mode for the font options object. 
 * 
 * Valid modes are:
 * 
 * + cairo.ANTIALIAS_DEFAULT - use the default antialiasing for the subsystem and target device.
 * + cairo.ANTIALIAS_NONE - use a bilevel alpha mask.
 * + cairo.ANTIALIAS_GRAY - perform single-color antialiasing (using shades of gray for black text on a white background, for example).
 * + cairo.ANTIALIAS_SUBPIXEL - perform antialiasing by taking advantage of the order of subpixel elements on devices such as LCD panels.
 * + cairo_ANTIALIAS_FAST - Hint that the backend should perform some antialiasing but prefer speed over quality, since 1.12.
 * + cairo.ANTIALIAS_GOOD - The backend should balance quality against performance, since 1.12.
 * + CAIRO_ANTIALIAS_BEST - Hint that the backend should render at the highest quality, sacrificing speed if necessary, since 1.12
 * 
 * @param {object} options - opaque handle to a font options object.
 * @return {int} mode - one of the above modes.
 */
static JSVAL font_options_get_antialias(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_options_get_antialias(options));
}

/**
 * @function cairo.font_options_set_subpixel_order
 * 
 * ### Synopsis
 * 
 * cairo.font_options_set_subpixel_order(options, subpixel_order);
 * 
 * Sets the subpixel order for the font options object. 
 * 
 * The subpixel order specifies the order of color elements within each pixel on the display device when rendering with an antialiasing mode of cairo.ANTIALIAS_SUBPIXEL.
 * 
 * The value of subpixel_order may be one of the following:
 * 
 * + cairo.SUBPIXEL_ORDER_DEFAULT - Use the default subpixel order for for the target device.
 * + cairo.SUBPIXEL_ORDER_RGB - Subpixel elements are arranged horizontally with red at the left.
 * + cairo.SUBPIXEL_ORDER_BGR - Subpixel elements are arranged horizontally with blue at the left.
 * + cairo.SUBPIXEL_ORDER_VRGB - Subpixel elements are arranged vertically with red at the top.
 * + cairo.SUBPIXEL_ORDER_VBGR - Subpixel elements are arranged vertically with blue at the top.
 * 
 * @param {object} options - opaque handle to a font options object.
 * @return {int} subpixel_order - one of the above values.
 */
static JSVAL font_options_set_subpixel_order(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    cairo_font_options_set_subpixel_order(options, (cairo_subpixel_order_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.font_options_get_subpixel_order
 * 
 * ### Synopsis
 * 
 * var subpixel_order = cairo.font_options_get_subpixel_order(options);
 * 
 * Get the subpixel order for the font options object.
 * 
 * The return value may be one of:
 * 
 * + cairo.SUBPIXEL_ORDER_DEFAULT - Use the default subpixel order for for the target device.
 * + cairo.SUBPIXEL_ORDER_RGB - Subpixel elements are arranged horizontally with red at the left.
 * + cairo.SUBPIXEL_ORDER_BGR - Subpixel elements are arranged horizontally with blue at the left.
 * + cairo.SUBPIXEL_ORDER_VRGB - Subpixel elements are arranged vertically with red at the top.
 * + cairo.SUBPIXEL_ORDER_VBGR - Subpixel elements are arranged vertically with blue at the top.
 * 
 * @param {object} options - opaque handle to a font options object.
 * @return {int} subpixel_order - one of the above values.
 */
static JSVAL font_options_get_subpixel_order(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_options_get_subpixel_order(options));
}

/**
 * @function cairo.font_options_set_hint_style
 * 
 * ### Synopsis
 * 
 * cairo.font_options_set_hint_style(options, hint_style);
 * 
 * Sets the hint style for font outlines for the font options object. 
 * 
 * This controls whether to fit font outlines to the pixel grid, and if so, whether to optimize for fidelity or contrast. 
 * 
 * The value of hint_style may be one of the following:
 * 
 * + cairo.HINT_STYLE_DEFAULT - Use the default hint style for font backend and target device.
 * + cairo.HINT_STYLE_NONE - Do not hint outlines.
 * + cairo.HINT_STYLE_SLIGHT - Hint outlines slightly to improve contrast while retaining good fidelity to the original shapes.
 * + cairo.HINT_STYLE_MEDIUM - Hint outlines with medium strength giving a compromise between fidelity to the original shapes and contrast.
 * + cairo.HINT_STYLE_FULL - Hint outlines to maximize contrast.
 * 
 * @param {object} options - opaque handle to a font options object.
 * @return {int} hint_style - one of the above values.
 */
static JSVAL font_options_set_hint_style(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    cairo_font_options_set_hint_style(options, (cairo_hint_style_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.font_options_get_hint_style
 * 
 * ### Synopsis
 * 
 * var hint_style = cairo.font_options_get_hint_style(options);
 * 
 * Get the hint style for the font options object.
 * 
 * The return value may be one of:
 * 
 * + cairo.HINT_STYLE_DEFAULT - Use the default hint style for font backend and target device.
 * + cairo.HINT_STYLE_NONE - Do not hint outlines.
 * + cairo.HINT_STYLE_SLIGHT - Hint outlines slightly to improve contrast while retaining good fidelity to the original shapes.
 * + cairo.HINT_STYLE_MEDIUM - Hint outlines with medium strength giving a compromise between fidelity to the original shapes and contrast.
 * + cairo.HINT_STYLE_FULL - Hint outlines to maximize contrast.
 * 
 * @param {object} options - opaque handle to a font options object.
 * @return {int} hint_style - one of the above values.
 */
static JSVAL font_options_get_hint_style(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_options_get_hint_style(options));
}

/**
 * @function cairo.font_options_set_hint_metrics
 * 
 * ### Synopsis
 * 
 * cairo.font_options_set_hint_metrics(options, hint_metrics);
 * 
 * Sets the metrics hinting mode for the font options object. 
 * 
 * This controls whether metrics are quantized to integer values in device units.
 * 
 * Hinting font metrics means quantizing them so that they are integer values in device space. Doing this improves the consistency of letter and line spacing, however it also means that text will be laid out differently at different zoom factors.
 * 
 * The value of hint_metrics may be one of the following:
 * 
 * + cairo.HINT_METRICS_DEFAULT - Hint metrics in the default manner for the font backend and target device.
 * + cairo.HINT_METRICS_OFF - Do not hint font metrics.
 * + cairo.HINT_METRICS_ON - Hint font metrics.
 * 
 * @param {object} options - opaque handle to a font options object.
 * @return {int} hint_metrics - one of the above values.
 */
static JSVAL font_options_set_hint_metrics(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    cairo_font_options_set_hint_metrics(options, (cairo_hint_metrics_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.font_options_get_hint_metrics
 * 
 * ### Synopsis
 * 
 * var hint_metrics = cairo.font_options_get_hint_metrics(options);
 * 
 * Get the hint metrics for the font options object.
 * 
 * The return value may be one of:
 * 
 * + cairo.HINT_METRICS_DEFAULT - Hint metrics in the default manner for the font backend and target device.
 * + cairo.HINT_METRICS_OFF - Do not hint font metrics.
 * + cairo.HINT_METRICS_ON - Hint font metrics.
 * 
 * @param {object} options - opaque handle to a font options object.
 * @return {int} hint_metrics - one of the above values.
 */
static JSVAL font_options_get_hint_metrics(JSARGS args) {
    cairo_font_options_t *options = (cairo_font_options_t *)JSEXTERN(args[0]);
    return Integer::New(cairo_font_options_get_hint_metrics(options));
}

////////////////////////// PNG SUPPORT

/**
 * @function cairo.image_surface_create_from_png
 * 
 * ### Synopsis
 * 
 * var surface = cairo.image_surface_create_from_png(filename);
 * 
 * Creates a new image surface and initializes the contents to the given PNG file.
 * 
 * If an error occurs, this function returns a "nil" surface.  A nil surface can be checked for with cairo.surface_status(surface) which may return one of the following values: cairo.STATUS_NO_MEMORY, cairo.STATUS_FILE_NOT_FOUND, or  cairo.STATUS_READ_ERROR.
 * 
 * Alternatively, you can allow errors to propagate through the drawing operations and check the status on the context upon completion using cairo.context_status()..
 * 
 * @param {string} filename - name of PNG file to load.
 * @return {object} surface - opaque handle to a newly created surface.
 */
static JSVAL image_surface_create_from_png(JSARGS args) {
    String::Utf8Value filename(args[0]->ToString());
    return External::New(cairo_image_surface_create_from_png(*filename));
}

/**
 * @function cairo.surface_write_to_png
 * 
 * ### Synopsis
 * 
 * var status = cairo.surface_write_to_png(surface, filename);
 * 
 * Writes the contents of surface to a new file filename as a PNG image.
 * 
 * If an error occurs, the value returned may be:
 * 
 * + cairo.STATUS_NO_MEMORY if memory could not be allocated for the operation.
 * + cairo.STATUS_SURFACE_TYPE_MISMATCH if the surface does not have pixel contents.
 * + cairo.STATUS_WRITE_ERROR if an I/O error occurs while attempting to write the file..
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} status - either cairo.STATUS_SUCCESS, or one of the above values if an error occurred.
 */
static JSVAL surface_write_to_png(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    String::Utf8Value filename(args[1]->ToString());
    return Integer::New(cairo_surface_write_to_png(surface, *filename));
}

////////////////////////// PATTERNS
// http://www.cairographics.org/manual/cairo-cairo-pattern-t.html

/**
 * @function cairo.pattern_add_color_stop_rgb
 * 
 * ### Synopsis
 * 
 * cairo.pattern_add_color_stop_rgb(pattern, offset, red, green, blue);
 * 
 * Adds an opaque color stop to a gradient pattern. 
 * 
 * The offset specifies the location along the gradient's control vector. For example, a linear gradient's control vector is from (x0,y0) to (x1,y1) while a radial gradient's control vector is from any point on the start circle to the corresponding point on the end circle.
 * 
 * The color is specified in the same way as in cairo.context_set_source_rgb().
 * 
 * If two (or more) stops are specified with identical offset values, they will be sorted according to the order in which the stops are added, (stops added earlier will compare less than stops added later). This can be useful for reliably making sharp color transitions instead of the typical blend.
 * 
 * Note: If the pattern is not a gradient pattern, (eg. a linear or radial pattern), then the pattern will be put into an error status with a status of cairo.STATUS_PATTERN_TYPE_MISMATCH.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @param {number} offset - an offset in the range 0.0 ... 1.0
 * @param {number} red - red component of color.
 * @param {number} green - green component of color.
 * @param {number} blue - blue component of color.
 */
static JSVAL pattern_add_color_stop_rgb(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    cairo_pattern_add_color_stop_rgb(
        pattern,
        args[1]->NumberValue(),     // offset
        args[2]->NumberValue(),     // red
        args[3]->NumberValue(),     // green
        args[4]->NumberValue()      // blue
    );
    return Undefined();
}
        
/**
 * @function cairo.pattern_add_color_stop_rgba
 * 
 * ### Synopsis
 * 
 * cairo.pattern_add_color_stop_rgba(pattern, offset, red, green, blue, alpha);
 * 
 * Adds a translucent color stop to a gradient pattern. 
 * 
 * The offset specifies the location along the gradient's control vector. For example, a linear gradient's control vector is from (x0,y0) to (x1,y1) while a radial gradient's control vector is from any point on the start circle to the corresponding point on the end circle.
 * 
 * The color is specified in the same way as in cairo.context_set_source_rgba().
 * 
 * If two (or more) stops are specified with identical offset values, they will be sorted according to the order in which the stops are added, (stops added earlier will compare less than stops added later). This can be useful for reliably making sharp color transitions instead of the typical blend.
 * 
 * Note: If the pattern is not a gradient pattern, (eg. a linear or radial pattern), then the pattern will be put into an error status with a status of cairo.STATUS_PATTERN_TYPE_MISMATCH.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @param {number} offset - an offset in the range 0.0 ... 1.0
 * @param {number} red - red component of color.
 * @param {number} green - green component of color.
 * @param {number} blue - blue component of color.
 * @param {number} alpha - alpha component of color.
 */
static JSVAL pattern_add_color_stop_rgba(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    cairo_pattern_add_color_stop_rgba(
        pattern,
        args[1]->NumberValue(),     // offset
        args[2]->NumberValue(),     // red
        args[3]->NumberValue(),     // green
        args[4]->NumberValue(),     // blue
        args[5]->NumberValue()      // alpha
    );
    return Undefined();
}
        
/**
 * @function cairo.pattern_get_stop_color_count
 * 
 * ### Synopsis
 * 
 * var stops = cairo.pattern_get_stop_color_count(pattern);
 * 
 * Gets the number of color stops specified in the given gradient pattern.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {int} stops - number of color stops.
 * 
 * ### Note
 * 
 * This function will throw an exception if the pattern is not a gradient pattern.
 */
static JSVAL pattern_get_stop_color_count(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    int count;
    cairo_status_t status = cairo_pattern_get_color_stop_count(pattern, &count);
    if (status == CAIRO_STATUS_SUCCESS) {
        return Integer::New(count);
    }
    ThrowException(String::New(cairo_status_to_string(status)));
    return Undefined();
}

/**
 * @function cairo.pattern_get_color_stop_rgba
 * 
 * ### Synopsis
 * 
 * var stop = cairo.pattern_get_color_stop_rgba(pattern, index);
 * 
 * Gets the color and offset information at the given index for a gradient pattern. 
 * 
 * Values of index are 0 to (1 less than the number returned by cairo.pattern_get_color_stop_count()).
 * 
 * The object returned has the following members:
 * 
 * + {int} offset - offset of the stop
 * + {number} red - red component of the color, or NULL.
 * + {number} green - green component of the color, or NULL.
 * + {number} blue - blue component of the color, or NULL.
 * + {number} alpha - alpha component of the color, or NULL.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @param {int} index - index of the color stop information desired.
 * @return {object} stops - object of the form described above.
 * 
 * ### Note
 * 
 * This function will throw an exception if the index is invalid or the pattern is not a gradient pattern.
 */
static JSVAL pattern_get_color_stop_rgba(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    double offset, red, green, blue, alpha;
    cairo_status_t status = cairo_pattern_get_color_stop_rgba(pattern, args[1]->IntegerValue(), &offset, &red, &green, &blue, &alpha);
    if (status != CAIRO_STATUS_SUCCESS) {
        ThrowException(String::New(cairo_status_to_string(status)));
    }
    JSOBJ o = Object::New();
    o->Set(String::New("offset"), Integer::New(offset));
    o->Set(String::New("red"), Number::New(red));
    o->Set(String::New("green"), Number::New(green));
    o->Set(String::New("blue"), Number::New(blue));
    o->Set(String::New("alpha"), Number::New(alpha));
    return o;
}

/**
 * @function cairo.pattern_create_rgb
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.pattern_create_rgb(red, green, blue);
 * 
 * Creates a new pattern handle corresponding to an opaque color.  
 * 
 * The color components are floating point numbers in the range 0 to 1.  If the values passed in are outside that range, they will be clamped.
 * 
 * The caller owns the returned pattern handle and should call cairo.pattern_destroy() when finished with it.
 * 
 * This function will always return a valid handle, but if an error occurred, the pattern status will be set to an error.  To inspect the status of a pttern, use cairo.pattern_status().
 * 
 * @param {number} red - red component of color.
 * @param {number} green - green component of color.
 * @param {number} blue - blue component of color.
 * @return {object} pattern - opaque handle to newly created pattern.
 */
static JSVAL pattern_create_rgb(JSARGS args) {
    return External::New(cairo_pattern_create_rgb(
        args[1]->NumberValue(),     // red
        args[2]->NumberValue(),     // green
        args[3]->NumberValue()      // blue
     ));
}

/**
 * @function cairo.pattern_create_rgba
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.pattern_create_rgba(red, green, blue, alpha);
 * 
 * Creates a new pattern handle corresponding to a translucent color
 * 
 * The color components are floating point numbers in the range 0 to 1.  If the values passed in are outside that range, they will be clamped.
 * 
 * The caller owns the returned pattern handle and should call cairo.pattern_destroy() when finished with it.
 * 
 * This function will always return a valid handle, but if an error occurred, the pattern status will be set to an error.  To inspect the status of a pttern, use cairo.pattern_status().
 * 
 * @param {number} red - red component of color.
 * @param {number} green - green component of color.
 * @param {number} blue - blue component of color.
 * @param {number} alpha - alpha component of color.
 * @return {object} pattern - opaque handle to newly created pattern.
 */
static JSVAL pattern_create_rgba(JSARGS args) {
    return External::New(cairo_pattern_create_rgba(
        args[1]->NumberValue(),     // red
        args[2]->NumberValue(),     // green
        args[3]->NumberValue(),     // blue
        args[4]->NumberValue()      // alpha
     ));
}

/**
 * @function cairo.pattern_get_rgba
 * 
 * ### Synopsis
 * 
 * var color = cairo.pattern_get_rgba(pattern);
 * 
 * Gets the solid color for a solid color pattern.
 * 
 * The object returned has the following members:
 * 
 * + {number} red - red component of the color, or NULL.
 * + {number} green - green component of the color, or NULL.
 * + {number} blue - blue component of the color, or NULL.
 * + {number} alpha - alpha component of the color, or NULL.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {object} color - object of the form described above.
 * 
 * ### Note
 * 
 * This function will throw an exception if the pattern is not a solid color pattern.
 */
static JSVAL pattern_get_rgba(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    double red, green, blue, alpha;
    cairo_status_t status = cairo_pattern_get_rgba(pattern, &red, &green, &blue, &alpha);
    if (status != CAIRO_STATUS_SUCCESS) {
        ThrowException(String::New(cairo_status_to_string(status)));
    }
    JSOBJ o = Object::New();
    o->Set(String::New("red"), Number::New(red));
    o->Set(String::New("green"), Number::New(green));
    o->Set(String::New("blue"), Number::New(blue));
    o->Set(String::New("alpha"), Number::New(alpha));
    return o;
}

/**
 * @function cairo.pattern_create_for_surface
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.pattern_create_for_surface(surface);
 * 
 * Create a new pattern for the given surface.
 * 
 * The caller owns the returned handle and should call cairo.pattern_destroy() when finished with it.
 * 
 * This function will always return a valid pointer, but if an error occurred, the pattern status will be set to an error.  To inspect the status of a patter, use cairo.pattern_status().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL pattern_create_for_surface(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return External::New(cairo_pattern_create_for_surface(surface));
}

/**
 * @function cairo.pattern_get_surface
 * 
 * ### Synopsis
 * 
 * var surface = cairo.pattern_get_surface(pattern);
 * 
 * Get the surface of a surface pattern.
 * 
 * The reference returned in surface is owned by the pattern; the caller should call cairo.surface_reference() if the surface is to be retained.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {object} surface - opaque handle to a cairo surface.
 * 
 * ### Note
 * 
 * This function will throw an exception if the pattern is not a surface pattern.
 */
static JSVAL pattern_get_surface(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    cairo_surface_t *surface;
    cairo_status_t status = cairo_pattern_get_surface(pattern, &surface);
    if (status != CAIRO_STATUS_SUCCESS) {
        ThrowException(String::New(cairo_status_to_string(status)));
    }
    return External::New(surface);
}

/**
 * @function cairo.pattern_create_linear
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.pattern_create_linear(x0,y0, x1,y1);
 * 
 * Create a new linear gradient cairo_pattern_t along the line defined by (x0, y0) and (x1, y1). 
 * 
 * Before using the gradient pattern, a number of color stops should be defined using cairo.pattern_add_color_stop_rgb() or cairo.pattern_add_color_stop_rgba().
 * 
 * Note: The coordinates here are in pattern space. For a new pattern, pattern space is identical to user space, but the relationship between the spaces can be changed with cairo.pattern_set_matrix().
 * 
 * The caller owns the returned handle and should call cairo.pattern_destroy() when finished with it.
 * 
 * This function will always return a valid pointer, but if an error occurred, the pattern status will be set to an error.  To inspect the status of a patter, use cairo.pattern_status().
 * 
 * @param {number} x0 - x coordinate of the start point.
 * @param {number} y0 - y coordinate of the start point.
 * @param {number} x1 - x coordinate of the end point.
 * @param {number} y1 - y coordinate of the end point.
 * @return {object} pattern - opaque handle to newly created pattern.
 */
static JSVAL pattern_create_linear(JSARGS args) {
    return External::New(cairo_pattern_create_linear(
        args[0]->NumberValue(),     // x0
        args[1]->NumberValue(),     // y0
        args[2]->NumberValue(),     // x1
        args[3]->NumberValue()      // y1
    ));
}

/**
 * @function cairo.pattern_get_linear_points
 * 
 * ### Synopsis
 * 
 * var points = cairo.pattern_get_linear_points(pattern);
 * 
 * Gets the gradient endpoints for a linear gradient pattern.
 * 
 * The object returned has the following members:
 * 
 * + {number} x0 - x coordinate of the first point.
 * + {number} y0 - y coordinate of the first point.
 * + {number} x1 - x coordinate of the second point.
 * + {number} y1 - y coordinate of the second point.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {object} points - object of the form described above.
 * 
 * ### Note
 * 
 * This function will throw an exception if the pattern is not a linear gradient pattern.
 */
static JSVAL pattern_get_linear_points(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    double x0,y0, x1,y1;
    cairo_status_t status = cairo_pattern_get_linear_points(pattern, &x0,&y0, &x1,&y1);
    if (status != CAIRO_STATUS_SUCCESS) {
        ThrowException(String::New(cairo_status_to_string(status)));
    }
    JSOBJ o = Object::New();
    o->Set(String::New("x0"), Number::New(x0));
    o->Set(String::New("y0"), Number::New(y0));
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    return o;
}

/**
 * @function cairo.pattern_create_radial
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.pattern_create_radial(cx0,cy0,radius0, cx1,cy1,radius1);
 * 
 * Creates a new radial gradient cairo_pattern_t between the two circles defined by (cx0, cy0, radius0) and (cx1, cy1, radius1). 
 * 
 * Before using the gradient pattern, a number of color stops should be defined using cairo.pattern_add_color_stop_rgb() or cairo.pattern_add_color_stop_rgba().
 * 
 * Note: The coordinates here are in pattern space. For a new pattern, pattern space is identical to user space, but the relationship between the spaces can be changed with cairo.pattern_set_matrix().
 * 
 * The caller owns the returned handle and should call cairo.pattern_destroy() when finished with it.
 * 
 * This function will always return a valid pointer, but if an error occurred, the pattern status will be set to an error.  To inspect the status of a patter, use cairo.pattern_status().
 * 
 * @param {number} cx0 - x coordinate for the center of the start circle.
 * @param {number} cy0 - y coordinate for the center of the start circle.
 * @param {number} radius0 - radius of the start cirle.
 * @param {number} cx1 - x coordinate for the center of the end circle.
 * @param {number} cy1 - y coordinate for the center of the end circle.
 * @param {number} radius1 - radius of the end cirle.
 * @return {object} pattern - opaque handle to newly created pattern.
 */
static JSVAL pattern_create_radial(JSARGS args) {
    return External::New(cairo_pattern_create_radial(
        args[0]->NumberValue(),     // cx0
        args[1]->NumberValue(),     // cy0
        args[2]->NumberValue(),     // radius0
        args[3]->NumberValue(),     // cx1
        args[4]->NumberValue(),     // cy1
        args[5]->NumberValue()      // radius1
    ));
}

/**
 * @function cairo.pattern_get_radial_circles
 * 
 * ### Synopsis
 * 
 * var circles = cairo.pattern_get_radial_circles(pattern);
 * 
 * Gets the gradient endpoint circles for a radial gradient, each specified as a center coordinate and radius.
 * 
 * The object returned has the following members:
 * 
 * + {number} x0 - x coordinate of the center of the first circle.
 * + {number} y0 - y coordinate of the center of the first circle.
 * + {number} r0 - radius the first circle.
 * + {number} x1 - x coordinate of the center of the second circle.
 * + {number} y1 - y coordinate of the center of the second circle.
 * + {number} r1 - radius the second circle.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {object} circles - object of the form described above.
 * 
 * ### Note
 * 
 * This function will throw an exception if the pattern is not a solid color pattern.
 */
static JSVAL pattern_get_radial_circles(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    double x0,y0,r0, x1,y1,r1;
    cairo_status_t status = cairo_pattern_get_radial_circles(pattern, &x0,&y0,&r0, &x1,&y1,&r1);
    if (status != CAIRO_STATUS_SUCCESS) {
        ThrowException(String::New(cairo_status_to_string(status)));
    }
    JSOBJ o = Object::New();
    o->Set(String::New("x0"), Number::New(x0));
    o->Set(String::New("y0"), Number::New(y0));
    o->Set(String::New("r0"), Number::New(r0));
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    o->Set(String::New("r1"), Number::New(r1));
    return o;
}

/**
 * @function cairo.pattern_reference
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.pattern_reference(pattern);
 * 
 * Increases the reference count on pattern by one. 
 * 
 * This prevents pattern from being destroyed until a matching call to cairo_pattern_destroy() is made.
 * 
 * The number of references to a pattern can be get using cairo.pattern_get_reference_count().
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {object} pattern - opaque handle to the referenced cairo pattern.
 */
static JSVAL pattern_reference(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    return External::New(cairo_pattern_reference(pattern));
}

/**
 * @function cairo.pattern_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.pattern_status(pattern);
 * 
 * Return error status code for this pattern.
 * 
 * Return value may be cairo.STATUS_SUCCESS if there is no error.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {int} status - cairo.STATUS_SUCCESS, cairo.STATUS_NO_MEMORY, cairo.STATUS_INVALID_MATRIX, or cairo.STATUS_PATTERN_TYPE_MISMATCH.
 */
static JSVAL pattern_status(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_pattern_status(pattern));
}

/**
 * @function cairo.pattern_set_extend
 * 
 * ### Synopsis
 * 
 * cairo.pattern_set_extend(extend);
 * 
 * Sets the mode to be used for drawing outside the area of a pattern.
 * 
 * Values for the extend parameter may be:
 * 
 * cairo.EXTEND_NONE - pixels outside of the source pattern are fully transparent
 * cairo.EXTEND_REPEAT - the pattern is tiled by repeating
 * cairo.EXTEND_REFLECT - the pattern is tiled by reflecting at the edges (Implemented for surface patterns since 1.6)
 * cairo.EXTEND_PAD - pixels outside of the pattern copy the closest pixel from the source (Since 1.2; but only implemented for surface patterns since 1.6)
 * 
 * The extend value is used to describe how pattern color/alpha will be determined for areas "outside" the pattern's natural area, (for example, outside the surface bounds or outside the gradient geometry).
 * 
 * The default extend mode is cairo.EXTEND_NONE for surface patterns and cairo.EXTEND_PAD for gradient patterns.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @param {int} extend - one of the above values.
 */
static JSVAL pattern_set_extend(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    cairo_pattern_set_extend(pattern, (cairo_extend_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.pattern_get_extend
 * 
 * ### Synopsis
 * 
 * var extend = cairo.pattern_get_extend(pattern);
 * 
 * Gets the current extend mode for a pattern.
 * 
 * Values for the returned value may be:
 * 
 * cairo.EXTEND_NONE - pixels outside of the source pattern are fully transparent
 * cairo.EXTEND_REPEAT - the pattern is tiled by repeating
 * cairo.EXTEND_REFLECT - the pattern is tiled by reflecting at the edges (Implemented for surface patterns since 1.6)
 * cairo.EXTEND_PAD - pixels outside of the pattern copy the closest pixel from the source (Since 1.2; but only implemented for surface patterns since 1.6)
 * 
 * The extend value is used to describe how pattern color/alpha will be determined for areas "outside" the pattern's natural area, (for example, outside the surface bounds or outside the gradient geometry).
 * 
 * The default extend mode is cairo.EXTEND_NONE for surface patterns and cairo.EXTEND_PAD for gradient patterns.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {int} extend - one of the above values.
 */
static JSVAL pattern_get_extend(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_pattern_get_extend(pattern));
    
}

/**
 * @function cairo.pattern_set_filter
 * 
 * ### Synopsis
 * 
 * cairo.pattern_set_filter(pattern, filter);
 * 
 * Sets the filter to be used for resizing when using this pattern.
 * 
 * The filter parameter may be one of:
 * 
 * cairo.FILTER_FAST - A high-performance filter, with quality similar to cairo.FILTER_NEAREST
 * cairo.FILTER_GOOD - A reasonable-performance filter, with quality similar to cairo.FILTER_BILINEAR
 * cairo.FILTER_BEST - The highest-quality available, performance may not be suitable for interactive use.
 * cairo.FILTER_NEAREST - Nearest-neighbor filtering
 * cairo.FILTER_BILINEAR - Linear interpolation in two dimensions
 * cairo.FILTER_GAUSSIAN - This filter value is currently unimplemented, and should not be used in current code.
 * 
 * Note that you might want to control filtering even when you do not have an explicit cairo_pattern_t object, (for example when using cairo.context_set_source_surface()). In these cases, it is convenient to use cairo.context__get_source() to get access to the pattern that cairo creates implicitly. For example:
 * ```
 * cairo.context_set_source_surface (context, image, x, y);
 * cairo.pattern_set_filter (cairo_get_source (context), cairo.FILTER_NEAREST);
 * ```
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @param {int} filter - one of the above values.
 */
static JSVAL pattern_set_filter(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    cairo_pattern_set_filter(pattern, (cairo_filter_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.pattern_get_filter
 * 
 * ### Synopsis
 * 
 * var filter = cairo.pattern_get_filter(pattern);
 * 
 * Get the current filter for a pattern.
 * 
 * See cairo.pattern_set_filter for details.
 * 
 * The returned filter value may be one of:
 * 
 * cairo.FILTER_FAST - A high-performance filter, with quality similar to cairo.FILTER_NEAREST
 * cairo.FILTER_GOOD - A reasonable-performance filter, with quality similar to cairo.FILTER_BILINEAR
 * cairo.FILTER_BEST - The highest-quality available, performance may not be suitable for interactive use.
 * cairo.FILTER_NEAREST - Nearest-neighbor filtering
 * cairo.FILTER_BILINEAR - Linear interpolation in two dimensions
 * cairo.FILTER_GAUSSIAN - This filter value is currently unimplemented, and should not be used in current code.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {int} filter - one of the above values.
 */
static JSVAL pattern_get_filter(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_pattern_get_filter(pattern));
}

/**
 * @function cairo.pattern_set_matrix
 * 
 * ### Synopsis
 * 
 * cairo.pattern_set_matrix(pattern, matrix);
 * 
 * Sets the pattern's transformation matrix to matrix. This matrix is a transformation from user space to pattern space.
 * 
 * When a pattern is first created it always has the identity matrix for its transformation matrix, which means that pattern space is initially identical to user space.
 * 
 * Important: Please note that the direction of this transformation matrix is from user space to pattern space. This means that if you imagine the flow from a pattern to user space (and on to device space), then coordinates in that flow will be transformed by the inverse of the pattern matrix.
 * 
 * For example, if you want to make a pattern appear twice as large as it does by default the correct code to use is:
 * 
 * ```
 * var matrix = cairo.matrix_init_scale (0.5, 0.5);
 * cairo.pattern_set_matrix (pattern, matrix);
 * ```
 * 
 * Meanwhile, using values of 2.0 rather than 0.5 in the code above would cause the pattern to appear at half of its default size.
 * 
 * Also, please note the discussion of the user-space locking semantics of cairo.context_set_source().
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @param {object} matrix - opaque handle to a cairo matrix.
 */
static JSVAL pattern_set_matrix(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[1]);
    cairo_pattern_set_matrix(pattern, matrix);
    return Undefined();
}

/**
 * @function cairo.pattern_get_matrix
 * 
 * ### Synopsis
 * 
 * var matrix = cairo.pattern_get_matrix(pattern);
 * 
 * Gets the pattern's transformation matrix.
 * 
 * The matrix returned is owned by the caller and must be released by calling cairo.matrix_destroy() when it is no longer needed.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {object} matrix - opaque handle to a cairo matrix.
 */
static JSVAL pattern_get_matrix(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    cairo_matrix_t *matrix = new cairo_matrix_t;
    cairo_pattern_get_matrix(pattern, matrix);
    return External::New(matrix);
}

/**
 * @function cairo.pattern_get_type
 * 
 * ### Synopsis
 * 
 * var type = cairo.pattern_get_type(pattern);
 * 
 * Get the type of a pattern.
 * 
 * The type of a pattern is determined by the function used to create it. 
 * 
 * The cairo.pattern_create_rgb() and cairo.pattern_create_rgba() functions create SOLID patterns. 
 * 
 * The remaining cairo.pattern_create functions map to pattern types in obvious ways.
 * 
 * The pattern type can be queried with cairo.pattern_get_type()
 * 
 * Most cairo pattern functions can be called with a pattern of any type, (though trying to change the extend or filter for a solid pattern will have no effect). 
 * 
 * A notable exception is cairo.pattern_add_color_stop_rgb() and cairo.pattern_add_color_stop_rgba() which must only be called with gradient patterns (either LINEAR or RADIAL). 
 * 
 * Otherwise the pattern will be shutdown and put into an error state.
 * 
 * Currently, the types are:
 * 
 * cairo.PATTERN_TYPE_SOLID - The pattern is a solid (uniform) color. It may be opaque or translucent.
 * cairo.PATTERN_TYPE_SURFACE - The pattern is a based on a surface (an image).
 * cairo.PATTERN_TYPE_LINEAR - The pattern is a linear gradient.
 * cairo.PATTERN_TYPE_RADIAL - The pattern is a radial gradient.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {int} type - one of the above types.
 */
static JSVAL pattern_get_type(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_pattern_get_type(pattern));
}

/**
 * @function cairo.pattern_get_reference_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.pattern_get_reference_count(pattern);
 * 
 * Gets the current reference count of pattern.
 * 
 * @param {object} pattern - opaque handle to a cairo pattern.
 * @return {int} count - reference count.
 */
static JSVAL pattern_get_reference_count(JSARGS args) {
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_pattern_get_reference_count(pattern));
}

////////////////////////// MATRIX

/**
 * @function cairo.matrix_create
 * 
 * ### Synopsis
 * 
 * var matrix = cairo.matrix_create();
 * 
 * Create a matrix object.
 * 
 * Matrices are used throughout cairo to convert between different coordinate spaces.  A matrix holds an affine transformation, such as a scale, rotation, shear, or a combination of these.  The transformation of a point(x,y) is given by:
 * 
 * ```
 * x_new = xx * x + xy * y + x0;
 * y_new = yx * x + yy * y + y0;
 * 
 * ```
 * 
 * Internally, a matrix contains the following number values: xx, yx, xy, yy, x0, and y0.  JavaScript code will not directly manipulate these values; convenience methods are provided for that purpose.
 * 
 * The matrix returned is initialized to an identity transformation.
 * 
 * Matrices returned by the SilkJS cairo methods are owned by the caller and should be released by calling cairo.matrix_destroy().
 * 
 * @return {object} matrix - opaque handle to a matrix.
 */
static JSVAL matrix_create(JSARGS args) {
    cairo_matrix_t *matrix = new cairo_matrix_t;
    cairo_matrix_init_identity(matrix);
    return External::New(matrix);
}

/**
 * @function cairo.matrix_init
 * 
 * ### Synopsis
 * 
 * cairo.matrix_init(matrix, xx, yx, xy, yy, x0, y0);
 * 
 * Sets matrix to be the affine transformation given by xx, yx, xy, yy, x0, y0. 
 * 
 * The transformation is given by:
 * ```
 *  x_new = xx * x + xy * y + x0;
 *  y_new = yx * x + yy * y + y0;
 * ```
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {number} xx - xx component of the affine transformation.
 * @param {number} yx - yx component of the affine transformation.
 * @param {number} xy - xy component of the affine transformation.
 * @param {number} yy - yy component of the affine transformation.
 * @param {number} x0 - x translation component of the affine transformation.
 * @param {number} y0 - y translation component of the affine transformation.
 */
static JSVAL matrix_init(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_init(
        matrix, 
        args[1]->NumberValue(),     // xx
        args[2]->NumberValue(),     // yx
        args[3]->NumberValue(),     // xy
        args[4]->NumberValue(),     // yy
        args[5]->NumberValue(),     // x0
        args[6]->NumberValue()      // y0
    );
    return Undefined();
}

/**
 * @function cairo.matrix_clone
 * 
 * ### Synopsis
 * 
 * var clone = cairo.matrix_clone(matrix);
 * 
 * This is a convenience function that creates a new matrix object as a clone of the original.
 * 
 * All members of the clone have the same values as the original.
 * 
 * The clone matrix is owned by the caller and must be released with cairo.matrix_destroy().
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @return {object} clone - opaque handle to a new matrix.
 */
static JSVAL matrix_clone(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_t *clone = new cairo_matrix_t;
    memcpy(clone, matrix, sizeof(cairo_matrix_t));
    return External::New(clone);
}


/**
 * @function cairo.matrix_init_identity
 * 
 * ### Synopsis
 * 
 * cairo.matrix_init_identity(matrix);
 * 
 * Modifies matrix to be an identity transformation.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 */
static JSVAL matrix_init_identity(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_init_identity(matrix);
    return Undefined();
}

/**
 * @function cairo.matrix_init_translate
 * 
 * ### Synopsis
 * 
 * cairo.matrix_init_translate(matrix, tx,ty);
 * 
 * Initializes matrix to a transformation that translates by tx and ty in the x and y dimensions, respectively.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {number} tx - amount to translate in the x direction.
 * @param {number} ty - amount to translate in the y direction.
 */
static JSVAL matrix_init_translate(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_init_translate(matrix, args[1]->NumberValue(), args[2]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.matrix_init_scale
 * 
 * ### Synopsis
 * 
 * cairo.matrix_init_scale(matrix, sx,sy);
 * 
 * Initializes matrix to a transformation that scales by sx and sy in the x and y dimensions, respectively.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {number} sx - scale factor in the x direction.
 * @param {number} sy - scake factir in the y direction.
 */
static JSVAL matrix_init_scale(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_init_scale(matrix, args[1]->NumberValue(), args[2]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.matrix_init_rotate
 * 
 * ### Synopsis
 * 
 * cairo.matrix_init_rotate(matrix, radians);
 * 
 * Initializes matrix to a transformation that rotates by radians.
 * 
 * The direction of rotation is defined such that positive angles rotate in the direction from the postitive x axis toward the positive y axis.
 * 
 * With the default axis orientation of cairo, positive angles rotate in a clockwise direction.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {number} radians angle of rotation, in radians.
 */
static JSVAL matrix_init_rotate(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_init_rotate(matrix, args[1]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.matrix_translate
 * 
 * ### Synopsis
 * 
 * cairo.matrix_translate(matrix, tx, ty);
 * 
 * Applies a translateion by tx,ty to the transformation in matrix.
 * 
 * The effect of the new transformation is to first translate the coordinates by tx and ty, then apply the original transformation to the coordinates.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {number} tx - amount to translate in the x direction.
 * @param {number} ty - amount to translate in the y direction.
 */
static JSVAL matrix_translate(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_translate(matrix, args[1]->NumberValue(), args[2]->NumberValue());    
    return Undefined();
}

/**
 * @function cairo.matrix_scale
 * 
 * ### Synopsis
 * 
 * cairo.matrix_scale(matrix, sx, sy);
 * 
 * Applies scaling by sx,sy to the transformation in matrix.
 * 
 * The effect of the new transformation is to first scale the coordinates by sx and sy, then apply the original transformation to the coordinates.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {number} sx - scale factor in the x direction.
 * @param {number} sy - scale factor in the y direction.
 */
static JSVAL matrix_scale(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_scale(matrix, args[1]->NumberValue(), args[2]->NumberValue());    
    return Undefined();
}

/**
 * @function cairo.matrix_rotate
 * 
 * ### Synopsis
 * 
 * cairo.matrix_rotate(matrix, radians);
 * 
 * Applies rotation by radians to the transformation in matrix. 
 * 
 * The effect of the new transformation is to first rotate the coordinates by radians, then apply the original transformation to the coordinates.
 * 
 * The direction of rotation is defined such that positive angles rotate in the direction from the positive X axis toward the positive Y axis. 
 * 
 * With the default axis orientation of cairo, positive angles rotate in a clockwise direction.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {number} radians angle of rotation, in radians.
 */
static JSVAL matrix_rotate(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_rotate(matrix, args[1]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.matrix_invert
 * 
 * ### Synopsis
 * 
 * var status = cairo.matrix_invert(matrix);
 * 
 * Changes matrix to be the inverse of its original value. 
 * 
 * Not all transformation matrices have inverses; if the matrix collapses points together (it is degenerate), then it has no inverse and this function will fail.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @return {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_INVALID_MATRIX.
 */
static JSVAL matrix_invert(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_matrix_invert(matrix));
}

/**
 * @function cairo.matrix_multiply
 * 
 * ### Synopsis
 * 
 * var matrix = cairo.matrix_multipl(a, b);
 * 
 * Multiplies the affine transformations in a and b together and returns a new resulting matrix. 
 * 
 * The effect of the resulting transformation is to first apply the transformation in a to the coordinates and then apply the transformation in b to the coordinates.
 * 
 * It is allowable for result to be identical to either a or b.
 * 
 * @param {object} a - opaque handle to a matrix.
 * @param {object} b - opaque handle to a matrix.
 * @return {object} matrix - opaque handle to a matrix.
 */
static JSVAL matrix_multiply(JSARGS args) {
    cairo_matrix_t *a = (cairo_matrix_t *) JSEXTERN(args[0]);
    cairo_matrix_t *b = (cairo_matrix_t *) JSEXTERN(args[1]);
    cairo_matrix_t *result = new cairo_matrix_t;
    cairo_matrix_multiply(result, a, b);
    return External::New(result);
}

/**
 * @function cairo.matrix_transform_distance
 * 
 * ### Synopsis
 * 
 * var vector = cairo.matrix_transform_distance(matrix, vector);
 * 
 * Transforms the distance vector by matrix.
 * 
 * This is similar to cairo.matrix_transform_point() except the translation components of the transformation are ignored.
 * 
 * The calculation of the returned vector is as follows:
 * 
 * ```
 * dx2 = dx1 * a + dy1 * c;
 * dy2 = dx1 * b + dy1 * d;
 * ```
 * 
 * The input vector and output vector are the same object, of the form:
 * 
 * + {number} dx - x component of distance vector.
 * + {number} dy - y component of distance vector.
 * 
 * The dx,dy members are modified by this routine.
 * 
 * Affine transformations are position invariant, so the same vector always transforms to the same vector. 
 * 
 * If (x1,y1) transforms to (x2,y2) then (x1+dx1,y1+dy1) will transform to (x1+dx2,y1+dy2) for all values of x1 and x2.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {object} vector - object with dx,dy input values.
 * @return {object} vector - object with modified dx,dy values.
 */
static JSVAL matrix_transform_distance(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    Local<String>_dx = String::New("dx");
    Local<String>_dy = String::New("dy");
    JSOBJ o = args[1]->ToObject();
    double dx = o->Get(_dx)->NumberValue();
    double dy = o->Get(_dy)->NumberValue();
    cairo_matrix_transform_distance(matrix, &dx, &dy);
    o->Set(_dx, Number::New(dx));
    o->Set(_dy, Number::New(dy));
    return o;
}

/**
 * @function cairo.matrix_transform_distance
 * 
 * ### Synopsis
 * 
 * var point = cairo.matrix_transform_point(matrix, point);
 * 
 * Transforms the point by matrix.
 * 
 * The input point and output point are the same object, of the form:
 * 
 * + {number} x - x position.
 * + {number} y - y position.
 * 
 * The x,y members are modified by this routine.
 * 
 * @param {object} matrix - opaque handle to a matrix.
 * @param {object} point - object with x,y input values.
 * @return {object} point - object with modified x,y values.
 */
static JSVAL matrix_transform_point(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    JSOBJ o = args[1]->ToObject();
    double x = o->Get(_x)->NumberValue();
    double y = o->Get(_y)->NumberValue();
    cairo_matrix_transform_point(matrix, &x, &y);
    o->Set(_x, Number::New(x));
    o->Set(_y, Number::New(y));
    return o;
}

/**
 * @function cairo.matrix_destroy
 * 
 * ### Synopsis
 * 
 * cairo.matrix_destroy(matrix);
 * 
 * Free resources used by a matrix.
 */
static JSVAL matrix_destroy(JSARGS args) {
    cairo_matrix_t *matrix = (cairo_matrix_t *) JSEXTERN(args[0]);
    delete matrix;
    return Undefined();
}


////////////////////////// REGION

/**
 * @function cairo.region_create
 * 
 * ### Synopsis
 * 
 * var region = cairo.region_create();
 * 
 * Allocates a new empty region object.
 * 
 * This function returns a newly allocated region object. 
 * 
 * Free with cairo.region_destroy(). 
 * 
 * This function always returns a valid object; if memory cannot be allocated, then a special error object is returned where all operations on the object do nothing. You can check for this with cairo.region_status().
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @return {object} region - opaque handle to a region
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_create(JSARGS args) {
    return External::New(cairo_region_create());
}
#endif

/**
 * @function cairo.region_create_rectangle
 * 
 * ### Synopsis
 * 
 * var region = cairo.region_create_rectangle(rectangle);
 * 
 * Allocates a new region containing the rectangle specified.
 * 
 * The rectangle argument is a JavaScript object of the form:
 * 
 * + {int} x - x coordinate of upper left corner of rectangle.
 * + {int} y - y coordinate of upper left corner of rectangle.
 * + {int} width - width of rectangle.
 * + {int} height - height of rectangle.
 * 
 * This function returns a newly allocated region object containing rectangle. 
 * 
 * Free with cairo.region_destroy(). 
 * 
 * This function always returns a valid object; if memory cannot be allocated, then a special error object is returned where all operations on the object do nothing. You can check for this with cairo.region_status().
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} rectangle - object of the above form.
 * @return {object} region - opaque handle to a region
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_create_rectangle(JSARGS args) {
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    Local<String>_w = String::New("width");
    Local<String>_h = String::New("height");
    JSOBJ o = args[0]->ToObject();
    cairo_rectangle_int_t rect = { 
        o->Get(_x)->IntegerValue(),
        o->Get(_y)->IntegerValue(),
        o->Get(_w)->IntegerValue(),
        o->Get(_h)->IntegerValue()
    };
    return External::New(cairo_region_create_rectangle(&rect));
}
#endif

/**
 * @function cairo.region_create_rectangles
 * 
 * ### Synopsis
 * 
 * var region = cairo.region_create_rectangle(rectangles);
 * 
 * Allocates a new region containing the rectangle specified.
 * 
 * The rectangle argument is an array of JavaScript objects of the form:
 * 
 * + {int} x - x coordinate of upper left corner of rectangle.
 * + {int} y - y coordinate of upper left corner of rectangle.
 * + {int} width - width of rectangle.
 * + {int} height - height of rectangle.
 * 
 * This function returns a newly allocated region object containing the union of all given rectangles. 
 * 
 * Free with cairo.region_destroy(). 
 * 
 * This function always returns a valid object; if memory cannot be allocated, then a special error object is returned where all operations on the object do nothing. You can check for this with cairo.region_status().
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {array} rectangles - array of rectangle objects of the above form.
 * @return {object} region - opaque handle to a region
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_create_rectangles(JSARGS args) {
    Handle<Array>rectangles = Handle<Array>::Cast(args[0]->ToObject());
    int numRectangles = rectangles->Length();
    cairo_rectangle_int_t *rects = new cairo_rectangle_int_t[numRectangles];

    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    Local<String>_w = String::New("width");
    Local<String>_h = String::New("height");

    for (int i=0; i<numRectangles; i++) {
        JSOBJ o = rectangles->Get(i)->ToObject();
        rects[i].x = o->Get(_x)->IntegerValue();
        rects[i].y = o->Get(_y)->IntegerValue();
        rects[i].width = o->Get(_w)->IntegerValue();
        rects[i].height = o->Get(_h)->IntegerValue();
    }
    cairo_region_t *region = cairo_region_create_rectangles(rects, numRectangles);
    delete [] rects;
    return External::New(region);
    
}
#endif

/**
 * @function cairo.region_copy
 * 
 * ### Synopsis
 * 
 * var copy = cairo.region_copy(original);
 * 
 * Allocates a new region object, copying the area from original.
 * 
 * This function returns a newly allocated region object containing the union of all given rectangles. 
 * 
 * Free with cairo.region_destroy(). 
 * 
 * This function always returns a valid object; if memory cannot be allocated, then a special error object is returned where all operations on the object do nothing. You can check for this with cairo.region_status().
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} original - opaque handle to a region.
 * @return {object} copy - opaque handle to a copy of the original.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_copy(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    return External::New(cairo_region_copy(region));
}
#endif

/**
 * @function cairo.region_reference
 * 
 * ### Synopsis
 * 
 * var region = cairo.region_reference(region);
 * 
 * Increases the reference count on region by one.
 * 
 * This prevents region from being destroyed until a matching call to cairo.region_destroy() is made.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region.
 * @return {object} region - opaque handle to the referenced region.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_reference(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    return External::New(cairo_region_reference(region));
}
#endif

/**
 * @function cairo.region_destroy
 * 
 * ### Synopsis
 * 
 * cairo.region_destroy(region);
 * 
 * Destroys a region created with cairo.region_create(), cairo.region_copy(), or cairo.region_create_rectangle(), etc.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_destroy(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    cairo_region_destroy(region);
    return Undefined();
}
#endif

/**
 * @function cairo.region_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_status(region);
 * 
 * Checks whether an error has previously occurred for this region object.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region.
 * @return {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_status(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_region_status(region));
}
#endif

/**
 * @function cairo.region_get_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.region_get_extents(region);
 * 
 * Gets the bounding rectangle of a region.
 * 
 * The object returned has the following members:
 * 
 * + x: x coordinate of the top left corner of the resulting extents.
 * + y: y coordinate of the top left corner of the resulting extents.
 * + width: width of resulting extents.
 * + height: height of resulting extents.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region.
 * @return {object} extents - see object description above.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_get_extents(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    cairo_rectangle_int_t rect;
    cairo_region_get_extents(region, &rect);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x"), Number::New(rect.x));
    o->Set(String::New("y"), Number::New(rect.y));
    o->Set(String::New("width"), Number::New(rect.width));
    o->Set(String::New("height"), Number::New(rect.height));
    return o;
}
#endif

/**
 * @function cairo.region_num_rectangles
 * 
 * ### Synopsis
 * 
 * var num = cairo.region_num_rectangles(region);
 * 
 * Get the number of rectangles contained in region.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region.
 * @return {int} num - number of rectangles contained in region.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_num_rectangles(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_region_num_rectangles(region));
}
#endif

/**
 * @function cairo.region_get_rectangle
 * 
 * ### Synopsis
 * 
 * var rectangle = cairo.region_get_rectangle(region);
 * 
 * Gets the nth rectangle from the region.
 * 
 * The object returned has the following members:
 * 
 * + x: x coordinate of the top left corner of the resulting extents.
 * + y: y coordinate of the top left corner of the resulting extents.
 * + width: width of resulting extents.
 * + height: height of resulting extents.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region.
 * @return {object} rectangle - see object description above.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_get_rectangle(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    int nth = args[1]->IntegerValue();
    cairo_rectangle_int_t rect;
    cairo_region_get_rectangle(region, nth, &rect);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x"), Number::New(rect.x));
    o->Set(String::New("y"), Number::New(rect.y));
    o->Set(String::New("width"), Number::New(rect.width));
    o->Set(String::New("height"), Number::New(rect.height));
    return o;
}
#endif

/**
 * @function cairo.region_is_empty
 * 
 * ### Synopsis
 * 
 * var is_empty = cairo.region_is_empty(region);
 * 
 * Determine if a region is empty.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region.
 * @return {boolean} is_empty - true if region is empty, false if not.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_is_empty(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    return cairo_region_is_empty(region) ? True() : False();
}
#endif

/**
 * @function cairo.region_contains_poinrt
 * 
 * ### Synopsis
 * 
 * var contains_point = cairo.region_contains_point(region, x, y);
 * 
 * Determine if the region contains the specified point (x,y).
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region.
 * @param {int} x - x coordinate of the point.
 * @param {int} y - y coordinate of the point.
 * @return {boolean} is_empty - true if region is empty, false if not.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_contains_point(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    return cairo_region_contains_point(region, args[1]->IntegerValue(), args[1]->IntegerValue()) ? True() : False();
}
#endif

/**
 * @function cairo.region_contains_rectangle
 * 
 * ### Synopsis
 * 
 * var overlap = cairo.region_contains_rectangle(region, rectangle);
 * 
 * Determine wither rectangle is inside, outside, or partially contained in region.
 * 
 * The rectangle argument is a JavaScript object of the form:
 * 
 * + {int} x - x coordinate of upper left corner of rectangle.
 * + {int} y - y coordinate of upper left corner of rectangle.
 * + {int} width - width of rectangle.
 * + {int} height - height of rectangle.
 * 
 * The overlap value returned is one of the following constants:
 * 
 * cairo.REGION_OVERLAP_IN - rectangle is completely inside region.
 * cairo.REGION_OVERLAP_OUT - rectangle is completely outside region.
 * cairo.REGION_OVERLAP_PART - rectiangle is partially inside and partially outside region.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region
 * @param {object} rectangle - object of the above form.
 * @return {int} overlap - one of the above values.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_contains_rectangle(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    Local<String>_w = String::New("width");
    Local<String>_h = String::New("height");
    JSOBJ o = args[0]->ToObject();
    cairo_rectangle_int_t rect = { 
        o->Get(_x)->IntegerValue(),
        o->Get(_y)->IntegerValue(),
        o->Get(_w)->IntegerValue(),
        o->Get(_h)->IntegerValue()
    };
    return Integer::New(cairo_region_contains_rectangle(region, &rect));
}
#endif

/**
 * @function cairo.region_equal
 * 
 * ### Synopsis
 * 
 * var is_equal = cairo.region_equal(a, b);
 * 
 * Determine wither two regions are equivalent.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} a - opaque handle to a region.
 * @param {object} b - opaque handle to a region.
 * @return {boolean} is_equal - true if both regions contain the same coverage, false if not.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_equal(JSARGS args) {
    cairo_region_t *a = (cairo_region_t *) JSEXTERN(args[0]);
    cairo_region_t *b = (cairo_region_t *) JSEXTERN(args[1]);
    return cairo_region_equal(a, b) ? True() : False();
}
#endif

/**
 * @function cairo.region_translate
 * 
 * ### Synopsis
 * 
 * cairo.region_translate(region, dx,dy);
 * 
 * Translate region by (dx,dy).
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} region - opaque handle to a region
 * @param {int} dx - amount to translate in the x direction.
 * @param {int} dy - amount to translate in the y direction.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_translate(JSARGS args) {
    cairo_region_t *region = (cairo_region_t *) JSEXTERN(args[0]);
    cairo_region_translate(region, args[1]->IntegerValue(), args[2]->IntegerValue());
    return Undefined();
}
#endif

/**
 * @function cairo.region_intersect
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_intersect(dst, other);
 * 
 * Computes the intersection of dst with other and places the result in dst.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} dst - opaque handle to a region.
 * @param {object} other - opaque handle to a region.
 * @param {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_intersect(JSARGS args) {
    cairo_region_t *dst = (cairo_region_t *) JSEXTERN(args[0]);
    cairo_region_t *other = (cairo_region_t *) JSEXTERN(args[1]);
    return Integer::New(cairo_region_intersect(dst, other));
}
#endif

/**
 * @function cairo.region_intersect_rectangle
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_intersect_rectangle(dst, rectangle);
 * 
 * Computes the intersection of dst with rectangle and places the result in dst.
 * 
 * The rectangle argument is a JavaScript object of the form:
 * 
 * + {int} x - x coordinate of upper left corner of rectangle.
 * + {int} y - y coordinate of upper left corner of rectangle.
 * + {int} width - width of rectangle.
 * + {int} height - height of rectangle.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} dst - opaque handle to a region.
 * @param {object} rectangle - object of the above form.
 * @param {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_intersect_rectangle(JSARGS args) {
    cairo_region_t *dst = (cairo_region_t *) JSEXTERN(args[0]);
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    Local<String>_w = String::New("width");
    Local<String>_h = String::New("height");
    JSOBJ o = args[0]->ToObject();
    cairo_rectangle_int_t rect = { 
        o->Get(_x)->IntegerValue(),
        o->Get(_y)->IntegerValue(),
        o->Get(_w)->IntegerValue(),
        o->Get(_h)->IntegerValue()
    };
    return Integer::New(cairo_region_intersect_rectangle(dst, &rect));
}
#endif

/**
 * @function cairo.region_subtract
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_subtract(dst, other);
 * 
 * Subtracts other from dst and places the result in dst.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} dst - opaque handle to a region.
 * @param {object} other - opaque handle to a region.
 * @param {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_subtract(JSARGS args) {
    cairo_region_t *dst = (cairo_region_t *) JSEXTERN(args[0]);
    cairo_region_t *other = (cairo_region_t *) JSEXTERN(args[1]);
    return Integer::New(cairo_region_subtract(dst, other));
}
#endif

/**
 * @function cairo.region_subtract_rectangle
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_subtract_rectangle(dst, rectangle);
 * 
 * Subtracts rectangle from dst and places the result in dst.
 * 
 * The rectangle argument is a JavaScript object of the form:
 * 
 * + {int} x - x coordinate of upper left corner of rectangle.
 * + {int} y - y coordinate of upper left corner of rectangle.
 * + {int} width - width of rectangle.
 * + {int} height - height of rectangle.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} dst - opaque handle to a region.
 * @param {object} rectangle - object of the above form.
 * @param {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_subtract_rectangle(JSARGS args) {
    cairo_region_t *dst = (cairo_region_t *) JSEXTERN(args[0]);
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    Local<String>_w = String::New("width");
    Local<String>_h = String::New("height");
    JSOBJ o = args[0]->ToObject();
    cairo_rectangle_int_t rect = { 
        o->Get(_x)->IntegerValue(),
        o->Get(_y)->IntegerValue(),
        o->Get(_w)->IntegerValue(),
        o->Get(_h)->IntegerValue()
    };
    return Integer::New(cairo_region_subtract_rectangle(dst, &rect));
}
#endif

/**
 * @function cairo.region_union
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_union(dst, other);
 * 
 * Computes the union of dst with other and places the result in dst.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} dst - opaque handle to a region.
 * @param {object} other - opaque handle to a region.
 * @param {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_union(JSARGS args) {
    cairo_region_t *dst = (cairo_region_t *) JSEXTERN(args[0]);
    cairo_region_t *other = (cairo_region_t *) JSEXTERN(args[1]);
    return Integer::New(cairo_region_union(dst, other));
}
#endif

/**
 * @function cairo.region_union_rectangle
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_union_rectangle(dst, rectangle);
 * 
 * Computes the union of dst with rectangle and places the result in dst.
 * 
 * The rectangle argument is a JavaScript object of the form:
 * 
 * + {int} x - x coordinate of upper left corner of rectangle.
 * + {int} y - y coordinate of upper left corner of rectangle.
 * + {int} width - width of rectangle.
 * + {int} height - height of rectangle.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} dst - opaque handle to a region.
 * @param {object} rectangle - object of the above form.
 * @param {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_union_rectangle(JSARGS args) {
    cairo_region_t *dst = (cairo_region_t *) JSEXTERN(args[0]);
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    Local<String>_w = String::New("width");
    Local<String>_h = String::New("height");
    JSOBJ o = args[0]->ToObject();
    cairo_rectangle_int_t rect = { 
        o->Get(_x)->IntegerValue(),
        o->Get(_y)->IntegerValue(),
        o->Get(_w)->IntegerValue(),
        o->Get(_h)->IntegerValue()
    };
    return Integer::New(cairo_region_union_rectangle(dst, &rect));
}
#endif

/**
 * @function cairo.region_xor
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_xor(dst, other);
 * 
 * Computes the exclusive difference of dst with other and places the result in dst. 
 * 
 * That is, dst will be set to contain all areas that are either in dst or in other, but not in both.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} dst - opaque handle to a region.
 * @param {object} other - opaque handle to a region.
 * @param {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_xor(JSARGS args) {
    cairo_region_t *dst = (cairo_region_t *) JSEXTERN(args[0]);
    cairo_region_t *other = (cairo_region_t *) JSEXTERN(args[1]);
    return Integer::New(cairo_region_xor(dst, other));
}
#endif

/**
 * @function cairo.region_xor_rectangle
 * 
 * ### Synopsis
 * 
 * var status = cairo.region_xor_rectangle(dst, rectangle);
 * 
 * Computes the exclusive difference of dst with rectangle and places the result in dst. 
 * 
 * That is, dst will be set to contain all areas that are either in dst or in rectangle, but not in both.
 * 
 * The rectangle argument is a JavaScript object of the form:
 * 
 * + {int} x - x coordinate of upper left corner of rectangle.
 * + {int} y - y coordinate of upper left corner of rectangle.
 * + {int} width - width of rectangle.
 * + {int} height - height of rectangle.
 * 
 * AVAILABLE IN CAIRO 1.10 OR NEWER
 * 
 * @param {object} dst - opaque handle to a region.
 * @param {object} rectangle - object of the above form.
 * @param {int} status - one of cairo.STATUS_SUCCESS or cairo.STATUS_NO_MEMORY.
 */
#if CAIRO_VERSION_MINOR >= 10
static JSVAL region_xor_rectangle(JSARGS args) {
    cairo_region_t *dst = (cairo_region_t *) JSEXTERN(args[0]);
    Local<String>_x = String::New("x");
    Local<String>_y = String::New("y");
    Local<String>_w = String::New("width");
    Local<String>_h = String::New("height");
    JSOBJ o = args[0]->ToObject();
    cairo_rectangle_int_t rect = { 
        o->Get(_x)->IntegerValue(),
        o->Get(_y)->IntegerValue(),
        o->Get(_w)->IntegerValue(),
        o->Get(_h)->IntegerValue()
    };
    return Integer::New(cairo_region_xor_rectangle(dst, &rect));
}
#endif


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\///\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\///\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\///\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\///\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\///\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\///\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\///\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

extern "C" JSOBJ getExports () {
    Handle<ObjectTemplate>cairo = ObjectTemplate::New();

    cairo->Set(String::New("VERSION_MINOR"), Integer::New(CAIRO_VERSION_MINOR));

#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("FORMAT_INVALID"), Integer::New(CAIRO_FORMAT_INVALID));
#endif
    cairo->Set(String::New("FORMAT_ARGB32"), Integer::New(CAIRO_FORMAT_ARGB32));
    cairo->Set(String::New("FORMAT_RGB24"), Integer::New(CAIRO_FORMAT_RGB24));
    cairo->Set(String::New("FORMAT_A8"), Integer::New(CAIRO_FORMAT_A8));
    cairo->Set(String::New("FORMAT_A1"), Integer::New(CAIRO_FORMAT_A1));
    cairo->Set(String::New("FORMAT_RGB16_565"), Integer::New(CAIRO_FORMAT_RGB16_565));
    
    cairo->Set(String::New("STATUS_SUCCESS"), Integer::New(CAIRO_STATUS_SUCCESS));
    cairo->Set(String::New("STATUS_NO_MEMORY"), Integer::New(CAIRO_STATUS_NO_MEMORY));
    cairo->Set(String::New("STATUS_INVALID_RESTORE"), Integer::New(CAIRO_STATUS_INVALID_RESTORE));
    cairo->Set(String::New("STATUS_INVALID_POP_GROUP"), Integer::New(CAIRO_STATUS_INVALID_POP_GROUP));
    cairo->Set(String::New("STATUS_NO_CURRENT_POINT"), Integer::New(CAIRO_STATUS_NO_CURRENT_POINT));
    cairo->Set(String::New("STATUS_INVALID_MATRIX"), Integer::New(CAIRO_STATUS_INVALID_MATRIX));
    cairo->Set(String::New("STATUS_INVALID_STATUS"), Integer::New(CAIRO_STATUS_INVALID_STATUS));
    cairo->Set(String::New("STATUS_NULL_POINTER"), Integer::New(CAIRO_STATUS_NULL_POINTER));
    cairo->Set(String::New("STATUS_INVALID_STRING"), Integer::New(CAIRO_STATUS_INVALID_STRING));
    cairo->Set(String::New("STATUS_INVALID_PATH_DATA"), Integer::New(CAIRO_STATUS_INVALID_PATH_DATA));
    cairo->Set(String::New("STATUS_READ_ERROR"), Integer::New(CAIRO_STATUS_READ_ERROR));
    cairo->Set(String::New("STATUS_WRITE_ERROR"), Integer::New(CAIRO_STATUS_WRITE_ERROR));
    cairo->Set(String::New("STATUS_SURFACE_FINISHED"), Integer::New(CAIRO_STATUS_SURFACE_FINISHED));
    cairo->Set(String::New("STATUS_SURFACE_TYPE_MISMATCH"), Integer::New(CAIRO_STATUS_SURFACE_TYPE_MISMATCH));
    cairo->Set(String::New("STATUS_PATTERN_TYPE_MISMATCH"), Integer::New(CAIRO_STATUS_PATTERN_TYPE_MISMATCH));
    cairo->Set(String::New("STATUS_INVALID_CONTENT"), Integer::New(CAIRO_STATUS_INVALID_CONTENT));
    cairo->Set(String::New("STATUS_INVALID_FORMAT"), Integer::New(CAIRO_STATUS_INVALID_FORMAT));
    cairo->Set(String::New("STATUS_INVALID_VISUAL"), Integer::New(CAIRO_STATUS_INVALID_VISUAL));
    cairo->Set(String::New("STATUS_FILE_NOT_FOUND"), Integer::New(CAIRO_STATUS_FILE_NOT_FOUND));
    cairo->Set(String::New("STATUS_INVALID_DASH"), Integer::New(CAIRO_STATUS_INVALID_DASH));
    cairo->Set(String::New("STATUS_INVALID_DSC_COMMENT"), Integer::New(CAIRO_STATUS_INVALID_DSC_COMMENT));
    cairo->Set(String::New("STATUS_INVALID_INDEX"), Integer::New(CAIRO_STATUS_INVALID_INDEX));
    cairo->Set(String::New("STATUS_CLIP_NOT_REPRESENTABLE"), Integer::New(CAIRO_STATUS_CLIP_NOT_REPRESENTABLE));
    cairo->Set(String::New("STATUS_TEMP_FILE_ERROR"), Integer::New(CAIRO_STATUS_TEMP_FILE_ERROR));
    cairo->Set(String::New("STATUS_INVALID_STRIDE"), Integer::New(CAIRO_STATUS_INVALID_STRIDE));
    cairo->Set(String::New("STATUS_FONT_TYPE_MISMATCH"), Integer::New(CAIRO_STATUS_FONT_TYPE_MISMATCH));
    cairo->Set(String::New("STATUS_USER_FONT_IMMUTABLE"), Integer::New(CAIRO_STATUS_USER_FONT_IMMUTABLE));
    cairo->Set(String::New("STATUS_USER_FONT_ERROR"), Integer::New(CAIRO_STATUS_USER_FONT_ERROR));
    cairo->Set(String::New("STATUS_NEGATIVE_COUNT"), Integer::New(CAIRO_STATUS_NEGATIVE_COUNT));
    cairo->Set(String::New("STATUS_INVALID_CLUSTERS"), Integer::New(CAIRO_STATUS_INVALID_CLUSTERS));
    cairo->Set(String::New("STATUS_INVALID_SLANT"), Integer::New(CAIRO_STATUS_INVALID_SLANT));
    cairo->Set(String::New("STATUS_INVALID_WEIGHT"), Integer::New(CAIRO_STATUS_INVALID_WEIGHT));
#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("STATUS_INVALID_SIZE"), Integer::New(CAIRO_STATUS_INVALID_SIZE));
    cairo->Set(String::New("STATUS_USER_FONT_NOT_IMPLEMENTED"), Integer::New(CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED));
    cairo->Set(String::New("STATUS_DEVICE_TYPE_MISMATCH"), Integer::New(CAIRO_STATUS_DEVICE_TYPE_MISMATCH));
    cairo->Set(String::New("STATUS_DEVICE_ERROR"), Integer::New(CAIRO_STATUS_DEVICE_ERROR));
#endif
    cairo->Set(String::New("CONTENT_COLOR"), Integer::New(CAIRO_CONTENT_COLOR));
    cairo->Set(String::New("CONTENT_ALPHA"), Integer::New(CAIRO_CONTENT_ALPHA));
    cairo->Set(String::New("CONTENT_COLOR_ALPHA"), Integer::New(CAIRO_CONTENT_COLOR_ALPHA));
    
    cairo->Set(String::New("SURFACE_TYPE_IMAGE"), Integer::New(CAIRO_SURFACE_TYPE_IMAGE));
    cairo->Set(String::New("SURFACE_TYPE_PDF"), Integer::New(CAIRO_SURFACE_TYPE_PDF));
    cairo->Set(String::New("SURFACE_TYPE_PS"), Integer::New(CAIRO_SURFACE_TYPE_PS));
    cairo->Set(String::New("SURFACE_TYPE_XLIB"), Integer::New(CAIRO_SURFACE_TYPE_XLIB));
    cairo->Set(String::New("SURFACE_TYPE_XCB"), Integer::New(CAIRO_SURFACE_TYPE_XCB));
    cairo->Set(String::New("SURFACE_TYPE_GLITZ"), Integer::New(CAIRO_SURFACE_TYPE_GLITZ));
    cairo->Set(String::New("SURFACE_TYPE_QUARTZ"), Integer::New(CAIRO_SURFACE_TYPE_QUARTZ));
    cairo->Set(String::New("SURFACE_TYPE_WIN32"), Integer::New(CAIRO_SURFACE_TYPE_WIN32));
    cairo->Set(String::New("SURFACE_TYPE_BEOS"), Integer::New(CAIRO_SURFACE_TYPE_BEOS));
    cairo->Set(String::New("SURFACE_TYPE_DIRECTFB"), Integer::New(CAIRO_SURFACE_TYPE_DIRECTFB));
    cairo->Set(String::New("SURFACE_TYPE_SVG"), Integer::New(CAIRO_SURFACE_TYPE_SVG));
    cairo->Set(String::New("SURFACE_TYPE_OS2"), Integer::New(CAIRO_SURFACE_TYPE_OS2));
    cairo->Set(String::New("SURFACE_TYPE_WIN32_PRINTING"), Integer::New(CAIRO_SURFACE_TYPE_WIN32_PRINTING));
    cairo->Set(String::New("SURFACE_TYPE_QUARTZ_IMAGE"), Integer::New(CAIRO_SURFACE_TYPE_QUARTZ_IMAGE));
#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("SURFACE_TYPE_SCRIPT"), Integer::New(CAIRO_SURFACE_TYPE_SCRIPT));
    cairo->Set(String::New("SURFACE_TYPE_QT"), Integer::New(CAIRO_SURFACE_TYPE_QT));
    cairo->Set(String::New("SURFACE_TYPE_RECORDING"), Integer::New(CAIRO_SURFACE_TYPE_RECORDING));
    cairo->Set(String::New("SURFACE_TYPE_VG"), Integer::New(CAIRO_SURFACE_TYPE_VG));
    cairo->Set(String::New("SURFACE_TYPE_GL"), Integer::New(CAIRO_SURFACE_TYPE_GL));
    cairo->Set(String::New("SURFACE_TYPE_DRM"), Integer::New(CAIRO_SURFACE_TYPE_DRM));
    cairo->Set(String::New("SURFACE_TYPE_TEE"), Integer::New(CAIRO_SURFACE_TYPE_TEE));
    cairo->Set(String::New("SURFACE_TYPE_XML"), Integer::New(CAIRO_SURFACE_TYPE_XML));
    cairo->Set(String::New("SURFACE_TYPE_SKIA"), Integer::New(CAIRO_SURFACE_TYPE_SKIA));
    cairo->Set(String::New("SURFACE_TYPE_SUBSURFACE"), Integer::New(CAIRO_SURFACE_TYPE_SUBSURFACE));
#endif
    
#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("MIME_TYPE_JPEG"), String::New(CAIRO_MIME_TYPE_JPEG));
    cairo->Set(String::New("MIME_TYPE_PNG"), String::New(CAIRO_MIME_TYPE_PNG));
    cairo->Set(String::New("MIME_TYPE_JP2"), String::New(CAIRO_MIME_TYPE_JP2));
    cairo->Set(String::New("MIME_TYPE_URI"), String::New(CAIRO_MIME_TYPE_URI));
#endif
    
    cairo->Set(String::New("ANTIALIAS_DEFAULT"), Integer::New(CAIRO_ANTIALIAS_DEFAULT));
    cairo->Set(String::New("ANTIALIAS_NONE"), Integer::New(CAIRO_ANTIALIAS_NONE));
    cairo->Set(String::New("ANTIALIAS_GRAY"), Integer::New(CAIRO_ANTIALIAS_GRAY));
    cairo->Set(String::New("ANTIALIAS_SUBPIXEL"), Integer::New(CAIRO_ANTIALIAS_SUBPIXEL));
#if CAIRO_VERSION_MINOR >= 12
    cairo->Set(String::New("ANTIALIAS_FAST"), Integer::New(CAIRO_ANTIALIAS_FAST));
    cairo->Set(String::New("ANTIALIAS_GOOD"), Integer::New(CAIRO_ANTIALIAS_GOOD));
    cairo->Set(String::New("ANTIALIAS_BEST"), Integer::New(CAIRO_ANTIALIAS_BEST));
#endif
    
    cairo->Set(String::New("SUBPIXEL_ORDER_DEFAULT"), Integer::New(CAIRO_SUBPIXEL_ORDER_DEFAULT));
    cairo->Set(String::New("SUBPIXEL_ORDER_RGB"), Integer::New(CAIRO_SUBPIXEL_ORDER_RGB));
    cairo->Set(String::New("SUBPIXEL_ORDER_BGR"), Integer::New(CAIRO_SUBPIXEL_ORDER_BGR));
    cairo->Set(String::New("SUBPIXEL_ORDER_VRGB"), Integer::New(CAIRO_SUBPIXEL_ORDER_VRGB));
    cairo->Set(String::New("SUBPIXEL_ORDER_VBGR"), Integer::New(CAIRO_SUBPIXEL_ORDER_VBGR));
    
    cairo->Set(String::New("HINT_STYLE_DEFAULT"), Integer::New(CAIRO_HINT_STYLE_DEFAULT));
    cairo->Set(String::New("HINT_STYLE_NONE"), Integer::New(CAIRO_HINT_STYLE_NONE));
    cairo->Set(String::New("HINT_STYLE_SLIGHT"), Integer::New(CAIRO_HINT_STYLE_SLIGHT));
    cairo->Set(String::New("HINT_STYLE_MEDIUM"), Integer::New(CAIRO_HINT_STYLE_MEDIUM));
    cairo->Set(String::New("HINT_STYLE_FULL"), Integer::New(CAIRO_HINT_STYLE_FULL));
    
    cairo->Set(String::New("HINT_METRICS_DEFAULT"), Integer::New(CAIRO_HINT_METRICS_DEFAULT));
    cairo->Set(String::New("HINT_METRICS_OFF"), Integer::New(CAIRO_HINT_METRICS_OFF));
    cairo->Set(String::New("HINT_METRICS_ON"), Integer::New(CAIRO_HINT_METRICS_ON));
    
    cairo->Set(String::New("LINE_CAP_BUTT"), Integer::New(CAIRO_LINE_CAP_BUTT));
    cairo->Set(String::New("LINE_CAP_ROUND"), Integer::New(CAIRO_LINE_CAP_ROUND));
    cairo->Set(String::New("LINE_CAP_SQUARE"), Integer::New(CAIRO_LINE_CAP_SQUARE));
    
    cairo->Set(String::New("FILL_RULE_WINDING"), Integer::New(CAIRO_FILL_RULE_WINDING));
    cairo->Set(String::New("FILL_RULE_EVEN_ODD"), Integer::New(CAIRO_FILL_RULE_EVEN_ODD));
    
    cairo->Set(String::New("LINE_JOIN_MITER"), Integer::New(CAIRO_LINE_JOIN_MITER));
    cairo->Set(String::New("LINE_JOIN_ROUND"), Integer::New(CAIRO_LINE_JOIN_ROUND));
    cairo->Set(String::New("LINE_JOIN_BEVEL"), Integer::New(CAIRO_LINE_JOIN_BEVEL));
    
    cairo->Set(String::New("OPERATOR_CLEAR"), Integer::New(CAIRO_OPERATOR_CLEAR));
    cairo->Set(String::New("OPERATOR_SOURCE"), Integer::New(CAIRO_OPERATOR_SOURCE));
    cairo->Set(String::New("OPERATOR_OVER"), Integer::New(CAIRO_OPERATOR_OVER));
    cairo->Set(String::New("OPERATOR_IN"), Integer::New(CAIRO_OPERATOR_IN));
    cairo->Set(String::New("OPERATOR_OUT"), Integer::New(CAIRO_OPERATOR_OUT));
    cairo->Set(String::New("OPERATOR_ATOP"), Integer::New(CAIRO_OPERATOR_ATOP));
    cairo->Set(String::New("OPERATOR_DEST"), Integer::New(CAIRO_OPERATOR_DEST));
    cairo->Set(String::New("OPERATOR_DEST_OVER"), Integer::New(CAIRO_OPERATOR_DEST_OVER));
    cairo->Set(String::New("OPERATOR_DEST_IN"), Integer::New(CAIRO_OPERATOR_DEST_IN));
    cairo->Set(String::New("OPERATOR_DEST_OUT"), Integer::New(CAIRO_OPERATOR_DEST_OUT));
    cairo->Set(String::New("OPERATOR_DEST_ATOP"), Integer::New(CAIRO_OPERATOR_DEST_ATOP));
    cairo->Set(String::New("OPERATOR_XOR"), Integer::New(CAIRO_OPERATOR_XOR));
    cairo->Set(String::New("OPERATOR_ADD"), Integer::New(CAIRO_OPERATOR_ADD));
    cairo->Set(String::New("OPERATOR_SATURATE"), Integer::New(CAIRO_OPERATOR_SATURATE));
#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("OPERATOR_MULTIPLY"), Integer::New(CAIRO_OPERATOR_MULTIPLY));
    cairo->Set(String::New("OPERATOR_SCREEN"), Integer::New(CAIRO_OPERATOR_SCREEN));
    cairo->Set(String::New("OPERATOR_OVERLAY"), Integer::New(CAIRO_OPERATOR_OVERLAY));
    cairo->Set(String::New("OPERATOR_DARKEN"), Integer::New(CAIRO_OPERATOR_DARKEN));
    cairo->Set(String::New("OPERATOR_LIGHTEN"), Integer::New(CAIRO_OPERATOR_LIGHTEN));
    cairo->Set(String::New("OPERATOR_COLOR_DODGE"), Integer::New(CAIRO_OPERATOR_COLOR_DODGE));
    cairo->Set(String::New("OPERATOR_COLOR_BURN"), Integer::New(CAIRO_OPERATOR_COLOR_BURN));
    cairo->Set(String::New("OPERATOR_HARD_LIGHT"), Integer::New(CAIRO_OPERATOR_HARD_LIGHT));
    cairo->Set(String::New("OPERATOR_SOFT_LIGHT"), Integer::New(CAIRO_OPERATOR_SOFT_LIGHT));
    cairo->Set(String::New("OPERATOR_DIFFERENCE"), Integer::New(CAIRO_OPERATOR_DIFFERENCE));
    cairo->Set(String::New("OPERATOR_EXCLUSION"), Integer::New(CAIRO_OPERATOR_EXCLUSION));
    cairo->Set(String::New("OPERATOR_HSL_HUE"), Integer::New(CAIRO_OPERATOR_HSL_HUE));
    cairo->Set(String::New("OPERATOR_HSL_SATURATION"), Integer::New(CAIRO_OPERATOR_HSL_SATURATION));
    cairo->Set(String::New("OPERATOR_HSL_COLOR"), Integer::New(CAIRO_OPERATOR_HSL_COLOR));
    cairo->Set(String::New("OPERATOR_HSL_LUMINOSITY"), Integer::New(CAIRO_OPERATOR_HSL_LUMINOSITY));
#endif
    cairo->Set(String::New("EXTEND_NONE"), Integer::New(CAIRO_EXTEND_NONE));
    cairo->Set(String::New("EXTEND_REPEAT"), Integer::New(CAIRO_EXTEND_REPEAT));
    cairo->Set(String::New("EXTEND_REFLECT"), Integer::New(CAIRO_EXTEND_REFLECT));
    cairo->Set(String::New("EXTEND_PAD"), Integer::New(CAIRO_EXTEND_PAD));
    
    cairo->Set(String::New("FILTER_FAST"), Integer::New(CAIRO_FILTER_FAST));
    cairo->Set(String::New("FILTER_GOOD"), Integer::New(CAIRO_FILTER_GOOD));
    cairo->Set(String::New("FILTER_BEST"), Integer::New(CAIRO_FILTER_BEST));
    cairo->Set(String::New("FILTER_NEAREST"), Integer::New(CAIRO_FILTER_NEAREST));
    cairo->Set(String::New("FILTER_BILINEAR"), Integer::New(CAIRO_FILTER_BILINEAR));
    cairo->Set(String::New("FILTER_GAUSSIAN"), Integer::New(CAIRO_FILTER_GAUSSIAN));
    
    cairo->Set(String::New("PATTERN_TYPE_SOLID"), Integer::New(CAIRO_PATTERN_TYPE_SOLID));
    cairo->Set(String::New("PATTERN_TYPE_SURFACE"), Integer::New(CAIRO_PATTERN_TYPE_SURFACE));
    cairo->Set(String::New("PATTERN_TYPE_LINEAR"), Integer::New(CAIRO_PATTERN_TYPE_LINEAR));
    cairo->Set(String::New("PATTERN_TYPE_RADIAL"), Integer::New(CAIRO_PATTERN_TYPE_RADIAL));
    
    
#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("REGION_OVERLAP_IN"), Integer::New(CAIRO_REGION_OVERLAP_IN));
    cairo->Set(String::New("REGION_OVERLAP_OUT"), Integer::New(CAIRO_REGION_OVERLAP_OUT));
    cairo->Set(String::New("REGION_OVERLAP_PART"), Integer::New(CAIRO_REGION_OVERLAP_PART));
#endif
    
    cairo->Set(String::New("FONT_SLANT_NORMAL"), Integer::New(CAIRO_FONT_SLANT_NORMAL));
    cairo->Set(String::New("FONT_SLANT_ITALIC"), Integer::New(CAIRO_FONT_SLANT_ITALIC));
    cairo->Set(String::New("FONT_SLANT_OBLIQUE"), Integer::New(CAIRO_FONT_SLANT_OBLIQUE));
    
    cairo->Set(String::New("FONT_WEIGHT_NORMAL"), Integer::New(CAIRO_FONT_WEIGHT_NORMAL));
    cairo->Set(String::New("FONT_WEIGHT_BOLD"), Integer::New(CAIRO_FONT_WEIGHT_BOLD));

    cairo->Set(String::New("TEXT_CLUSTER_FLAG_BACKWARD"), Integer::New(CAIRO_TEXT_CLUSTER_FLAG_BACKWARD));
    
#if CAIRO_VERSION_MINOR >= 2
    cairo->Set(String::New("FONT_TYPE_TOY"), Integer::New(CAIRO_FONT_TYPE_TOY));
    cairo->Set(String::New("FONT_TYPE_FT"), Integer::New(CAIRO_FONT_TYPE_FT));
    cairo->Set(String::New("FONT_TYPE_WIN32"), Integer::New(CAIRO_FONT_TYPE_WIN32));
#endif
    
#if CAIRO_VERSION_MINOR >= 6
    cairo->Set(String::New("FONT_TYPE_QUARTZ"), Integer::New(CAIRO_FONT_TYPE_QUARTZ));
    cairo->Set(String::New("FONT_TYPE_ATSUI"), Integer::New(CAIRO_FONT_TYPE_QUARTZ));
#else
#if CAIRO_VERSION_MINOR >= 2
    cairo->Set(String::New("FONT_TYPE_QUARTZ"), Integer::New(CAIRO_FONT_TYPE_ATSUI));
    cairo->Set(String::New("FONT_TYPE_ATSUI"), Integer::New(CAIRO_FONT_TYPE_ATSUI));
#endif
#endif
#if CAIRO_VERSION_MINOR >= 8
    cairo->Set(String::New("FONT_TYPE_USER"), Integer::New(CAIRO_FONT_TYPE_USER));
#endif    
    
//    net->Set(String::New("sendFile"), FunctionTemplate::New(net_sendfile));

    cairo->Set(String::New("status_to_string"), FunctionTemplate::New(status_to_string));
    cairo->Set(String::New("surface_create_similar"), FunctionTemplate::New(surface_create_similar));
    cairo->Set(String::New("surface_reference"), FunctionTemplate::New(surface_reference));
    cairo->Set(String::New("surface_status"), FunctionTemplate::New(surface_status));
    cairo->Set(String::New("surface_destroy"), FunctionTemplate::New(surface_destroy));
    cairo->Set(String::New("surface_finish"), FunctionTemplate::New(surface_finish));
    cairo->Set(String::New("surface_flush"), FunctionTemplate::New(surface_flush));
#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("surface_get_device"), FunctionTemplate::New(surface_get_device));
#endif    
    cairo->Set(String::New("surface_get_font_options"), FunctionTemplate::New(surface_get_font_options));
    cairo->Set(String::New("surface_get_content"), FunctionTemplate::New(surface_get_content));
    cairo->Set(String::New("surface_mark_dirty"), FunctionTemplate::New(surface_mark_dirty));
    cairo->Set(String::New("surface_mark_dirty_rectangle"), FunctionTemplate::New(surface_mark_dirty_rectangle));
    cairo->Set(String::New("surface_set_device_offset"), FunctionTemplate::New(surface_set_device_offset));
    cairo->Set(String::New("surface_get_device_offset"), FunctionTemplate::New(surface_get_device_offset));
    cairo->Set(String::New("surface_set_fallback_resolution"), FunctionTemplate::New(surface_set_fallback_resolution));
    cairo->Set(String::New("surface_get_fallback_resolution"), FunctionTemplate::New(surface_get_fallback_resolution));
    cairo->Set(String::New("surface_get_type"), FunctionTemplate::New(surface_get_type));
    cairo->Set(String::New("surface_get_reference_count"), FunctionTemplate::New(surface_get_reference_count));
    cairo->Set(String::New("surface_copy_page"), FunctionTemplate::New(surface_copy_page));
    cairo->Set(String::New("surface_show_page"), FunctionTemplate::New(surface_show_page));
    cairo->Set(String::New("surface_has_show_text_glyphs"), FunctionTemplate::New(surface_has_show_text_glyphs));
    cairo->Set(String::New("image_surface_create"), FunctionTemplate::New(image_surface_create));
    cairo->Set(String::New("image_surface_get_format"), FunctionTemplate::New(image_surface_get_format));
    cairo->Set(String::New("image_surface_get_width"), FunctionTemplate::New(image_surface_get_width));
    cairo->Set(String::New("image_surface_get_height"), FunctionTemplate::New(image_surface_get_height));
    cairo->Set(String::New("image_surface_get_data"), FunctionTemplate::New(image_surface_get_data));
    cairo->Set(String::New("surface_blur"), FunctionTemplate::New(surface_blur));
    cairo->Set(String::New("context_create"), FunctionTemplate::New(context_create));
    cairo->Set(String::New("context_reference"), FunctionTemplate::New(context_reference));
    cairo->Set(String::New("context_get_reference_count"), FunctionTemplate::New(context_get_reference_count));
    cairo->Set(String::New("context_destroy"), FunctionTemplate::New(context_destroy));
    cairo->Set(String::New("context_status"), FunctionTemplate::New(context_status));
    cairo->Set(String::New("context_save"), FunctionTemplate::New(context_save));
    cairo->Set(String::New("context_restore"), FunctionTemplate::New(context_restore));
    cairo->Set(String::New("context_get_target"), FunctionTemplate::New(context_get_target));
    cairo->Set(String::New("context_push_group"), FunctionTemplate::New(context_push_group));
    cairo->Set(String::New("context_push_group_with_content"), FunctionTemplate::New(context_push_group_with_content));
    cairo->Set(String::New("context_pop_group"), FunctionTemplate::New(context_pop_group));
    cairo->Set(String::New("context_pop_group_to_source"), FunctionTemplate::New(context_pop_group_to_source));
    cairo->Set(String::New("context_get_group_target"), FunctionTemplate::New(context_get_group_target));
    cairo->Set(String::New("context_set_source_rgb"), FunctionTemplate::New(context_set_source_rgb));
    cairo->Set(String::New("context_set_source_rgba"), FunctionTemplate::New(context_set_source_rgba));
    cairo->Set(String::New("context_set_source"), FunctionTemplate::New(context_set_source));
    cairo->Set(String::New("context_set_source_surface"), FunctionTemplate::New(context_set_source_surface));
    cairo->Set(String::New("context_get_source"), FunctionTemplate::New(context_get_source));
    cairo->Set(String::New("context_set_antialias"), FunctionTemplate::New(context_set_antialias));
    cairo->Set(String::New("context_get_antialias"), FunctionTemplate::New(context_get_antialias));
    cairo->Set(String::New("context_set_dash"), FunctionTemplate::New(context_set_dash));
    cairo->Set(String::New("context_get_dash_count"), FunctionTemplate::New(context_get_dash_count));
    cairo->Set(String::New("context_get_dash"), FunctionTemplate::New(context_get_dash));
    cairo->Set(String::New("context_set_fill_rule"), FunctionTemplate::New(context_set_fill_rule));
    cairo->Set(String::New("context_get_fill_rule"), FunctionTemplate::New(context_get_fill_rule));
    cairo->Set(String::New("context_set_line_cap"), FunctionTemplate::New(context_set_line_cap));
    cairo->Set(String::New("context_get_line_cap"), FunctionTemplate::New(context_get_line_cap));
    cairo->Set(String::New("context_set_line_join"), FunctionTemplate::New(context_set_line_join));
    cairo->Set(String::New("context_get_line_join"), FunctionTemplate::New(context_get_line_join));
    cairo->Set(String::New("context_set_line_width"), FunctionTemplate::New(context_set_line_width));
    cairo->Set(String::New("context_get_line_width"), FunctionTemplate::New(context_get_line_width));
    cairo->Set(String::New("context_set_miter_limit"), FunctionTemplate::New(context_set_miter_limit));
    cairo->Set(String::New("context_get_miter_limit"), FunctionTemplate::New(context_get_miter_limit));
    cairo->Set(String::New("context_set_operator"), FunctionTemplate::New(context_set_operator));
    cairo->Set(String::New("context_get_operator"), FunctionTemplate::New(context_get_operator));
    cairo->Set(String::New("context_set_tolerance"), FunctionTemplate::New(context_set_tolerance));
    cairo->Set(String::New("context_get_tolerance"), FunctionTemplate::New(context_get_tolerance));
    cairo->Set(String::New("context_clip"), FunctionTemplate::New(context_clip));
    cairo->Set(String::New("context_clip_preserve"), FunctionTemplate::New(context_clip_preserve));
    cairo->Set(String::New("context_clip_extents"), FunctionTemplate::New(context_clip_extents));
#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("context_in_clip"), FunctionTemplate::New(context_in_clip));
#endif
    cairo->Set(String::New("context_reset_clip"), FunctionTemplate::New(context_reset_clip));
    cairo->Set(String::New("context_fill"), FunctionTemplate::New(context_fill));
    cairo->Set(String::New("context_fill_preserve"), FunctionTemplate::New(context_fill_preserve));
    cairo->Set(String::New("context_fill_extents"), FunctionTemplate::New(context_fill_extents));
    cairo->Set(String::New("context_in_fill"), FunctionTemplate::New(context_in_fill));
    cairo->Set(String::New("context_mask"), FunctionTemplate::New(context_mask));
    cairo->Set(String::New("context_mask_surface"), FunctionTemplate::New(context_mask_surface));
    cairo->Set(String::New("context_paint"), FunctionTemplate::New(context_paint));
    cairo->Set(String::New("context_paint_with_alpha"), FunctionTemplate::New(context_paint_with_alpha));
    cairo->Set(String::New("context_stroke"), FunctionTemplate::New(context_stroke));
    cairo->Set(String::New("context_stroke_preserve"), FunctionTemplate::New(context_stroke_preserve));
    cairo->Set(String::New("context_stroke_extents"), FunctionTemplate::New(context_stroke_extents));
    cairo->Set(String::New("context_in_stroke"), FunctionTemplate::New(context_in_stroke));
    cairo->Set(String::New("context_copy_page"), FunctionTemplate::New(context_copy_page));
    cairo->Set(String::New("context_show_page"), FunctionTemplate::New(context_show_page));
    cairo->Set(String::New("context_translate"), FunctionTemplate::New(context_translate));
    cairo->Set(String::New("context_scale"), FunctionTemplate::New(context_scale));
    cairo->Set(String::New("context_rotate"), FunctionTemplate::New(context_rotate));
    cairo->Set(String::New("context_transform"), FunctionTemplate::New(context_transform));
    cairo->Set(String::New("context_set_matrix"), FunctionTemplate::New(context_set_matrix));
    cairo->Set(String::New("context_get_matrix"), FunctionTemplate::New(context_get_matrix));
    cairo->Set(String::New("context_identity_matrix"), FunctionTemplate::New(context_identity_matrix));
    cairo->Set(String::New("context_user_to_device"), FunctionTemplate::New(context_user_to_device));
    cairo->Set(String::New("context_user_to_device_distance"), FunctionTemplate::New(context_user_to_device_distance));
    cairo->Set(String::New("context_device_to_user"), FunctionTemplate::New(context_device_to_user));
    cairo->Set(String::New("context_device_to_user_distance"), FunctionTemplate::New(context_device_to_user_distance));

    cairo->Set(String::New("context_copy_path"), FunctionTemplate::New(context_copy_path));
    cairo->Set(String::New("context_copy_path_flat"), FunctionTemplate::New(context_copy_path_flat));
    cairo->Set(String::New("context_append_path"), FunctionTemplate::New(context_append_path));
    cairo->Set(String::New("path_destroy"), FunctionTemplate::New(path_destroy));
    cairo->Set(String::New("context_has_current_point"), FunctionTemplate::New(context_has_current_point));
    cairo->Set(String::New("context_get_current_point"), FunctionTemplate::New(context_get_current_point));
    cairo->Set(String::New("context_new_path"), FunctionTemplate::New(context_new_path));
    cairo->Set(String::New("context_new_sub_path"), FunctionTemplate::New(context_new_sub_path));
    cairo->Set(String::New("context_close_path"), FunctionTemplate::New(context_close_path));
    cairo->Set(String::New("context_arc"), FunctionTemplate::New(context_arc));
    cairo->Set(String::New("context_arc_negative"), FunctionTemplate::New(context_arc_negative));
    cairo->Set(String::New("context_curve_to"), FunctionTemplate::New(context_curve_to));
    cairo->Set(String::New("context_line_to"), FunctionTemplate::New(context_line_to));
    cairo->Set(String::New("context_move_to"), FunctionTemplate::New(context_move_to));
    cairo->Set(String::New("context_rectangle"), FunctionTemplate::New(context_rectangle));
    cairo->Set(String::New("context_glyph_path"), FunctionTemplate::New(context_glyph_path));
    cairo->Set(String::New("context_text_path"), FunctionTemplate::New(context_text_path));
    cairo->Set(String::New("context_rel_curve_to"), FunctionTemplate::New(context_rel_curve_to));
    cairo->Set(String::New("context_rel_line_to"), FunctionTemplate::New(context_rel_line_to));
    cairo->Set(String::New("context_rel_move_to"), FunctionTemplate::New(context_rel_move_to));
    cairo->Set(String::New("context_path_extents"), FunctionTemplate::New(context_path_extents));
    cairo->Set(String::New("context_select_font_face"), FunctionTemplate::New(context_select_font_face));
    cairo->Set(String::New("context_set_font_size"), FunctionTemplate::New(context_set_font_size));
    cairo->Set(String::New("context_set_font_matrix"), FunctionTemplate::New(context_set_font_matrix));
    cairo->Set(String::New("context_get_font_matrix"), FunctionTemplate::New(context_get_font_matrix));
    cairo->Set(String::New("context_set_font_options"), FunctionTemplate::New(context_set_font_options));
    cairo->Set(String::New("context_get_font_options"), FunctionTemplate::New(context_get_font_options));
    cairo->Set(String::New("context_set_font_face"), FunctionTemplate::New(context_set_font_face));
    cairo->Set(String::New("context_get_font_face"), FunctionTemplate::New(context_get_font_face));
    cairo->Set(String::New("context_set_scaled_font"), FunctionTemplate::New(context_set_scaled_font));
    cairo->Set(String::New("context_get_scaled_font"), FunctionTemplate::New(context_get_scaled_font));
    cairo->Set(String::New("context_show_text"), FunctionTemplate::New(context_show_text));
    cairo->Set(String::New("context_show_glyphs"), FunctionTemplate::New(context_show_glyphs));
    cairo->Set(String::New("context_show_text_glyphs"), FunctionTemplate::New(context_show_text_glyphs));
    cairo->Set(String::New("context_font_extents"), FunctionTemplate::New(context_font_extents));
    cairo->Set(String::New("context_text_extents"), FunctionTemplate::New(context_text_extents));
    cairo->Set(String::New("context_glyph_extents"), FunctionTemplate::New(context_glyph_extents));
    cairo->Set(String::New("toy_font_face_create"), FunctionTemplate::New(toy_font_face_create));
    cairo->Set(String::New("toy_font_face_get_family"), FunctionTemplate::New(toy_font_face_get_family));
    cairo->Set(String::New("toy_font_face_get_slant"), FunctionTemplate::New(toy_font_face_get_slant));
    cairo->Set(String::New("toy_font_face_get_weight"), FunctionTemplate::New(toy_font_face_get_weight));
    cairo->Set(String::New("font_face_reference"), FunctionTemplate::New(font_face_reference));
    cairo->Set(String::New("font_face_destroy"), FunctionTemplate::New(font_face_destroy));
    cairo->Set(String::New("font_face_status"), FunctionTemplate::New(font_face_status));
    cairo->Set(String::New("font_face_get_type"), FunctionTemplate::New(font_face_get_type));
    cairo->Set(String::New("font_face_get_reference_count"), FunctionTemplate::New(font_face_get_reference_count));
    cairo->Set(String::New("scaled_font_create"), FunctionTemplate::New(scaled_font_create));
    cairo->Set(String::New("scaled_font_reference"), FunctionTemplate::New(scaled_font_reference));
    cairo->Set(String::New("scaled_font_destroy"), FunctionTemplate::New(scaled_font_destroy));
    cairo->Set(String::New("scaled_font_get_reference_count"), FunctionTemplate::New(scaled_font_get_reference_count));
    cairo->Set(String::New("scaled_font_status"), FunctionTemplate::New(scaled_font_status));
    cairo->Set(String::New("scaled_font_extents"), FunctionTemplate::New(scaled_font_extents));
    cairo->Set(String::New("scaled_font_text_extents"), FunctionTemplate::New(scaled_font_text_extents));
    cairo->Set(String::New("scaled_font_glyph_extents"), FunctionTemplate::New(scaled_font_glyph_extents));
    cairo->Set(String::New("scaled_font_get_font_face"), FunctionTemplate::New(scaled_font_get_font_face));
    cairo->Set(String::New("scaled_font_get_font_options"), FunctionTemplate::New(scaled_font_get_font_options));
    cairo->Set(String::New("scaled_font_get_font_matrix"), FunctionTemplate::New(scaled_font_get_font_matrix));
    cairo->Set(String::New("scaled_font_get_ctm"), FunctionTemplate::New(scaled_font_get_ctm));
    cairo->Set(String::New("scaled_font_get_scale_matrix"), FunctionTemplate::New(scaled_font_get_scale_matrix));
    cairo->Set(String::New("scaled_font_get_type"), FunctionTemplate::New(scaled_font_get_type));
    cairo->Set(String::New("font_options_create"), FunctionTemplate::New(font_options_create));
    cairo->Set(String::New("font_options_copy"), FunctionTemplate::New(font_options_copy));
    cairo->Set(String::New("font_options_destroy"), FunctionTemplate::New(font_options_destroy));
    cairo->Set(String::New("font_options_status"), FunctionTemplate::New(font_options_status));
    cairo->Set(String::New("font_options_merge"), FunctionTemplate::New(font_options_merge));
    cairo->Set(String::New("font_options_hash"), FunctionTemplate::New(font_options_hash));
    cairo->Set(String::New("font_options_equal"), FunctionTemplate::New(font_options_equal));
    cairo->Set(String::New("font_options_set_antialias"), FunctionTemplate::New(font_options_set_antialias));
    cairo->Set(String::New("font_options_get_antialias"), FunctionTemplate::New(font_options_get_antialias));
    cairo->Set(String::New("font_options_set_subpixel_order"), FunctionTemplate::New(font_options_set_subpixel_order));
    cairo->Set(String::New("font_options_get_subpixel_order"), FunctionTemplate::New(font_options_get_subpixel_order));
    cairo->Set(String::New("font_options_set_hint_style"), FunctionTemplate::New(font_options_set_hint_style));
    cairo->Set(String::New("font_options_get_hint_style"), FunctionTemplate::New(font_options_get_hint_style));
    cairo->Set(String::New("font_options_set_hint_metrics"), FunctionTemplate::New(font_options_set_hint_metrics));
    cairo->Set(String::New("font_options_get_hint_metrics"), FunctionTemplate::New(font_options_get_hint_metrics));
    cairo->Set(String::New("image_surface_create_from_png"), FunctionTemplate::New(image_surface_create_from_png));
    cairo->Set(String::New("surface_write_to_png"), FunctionTemplate::New(surface_write_to_png));
    cairo->Set(String::New("pattern_add_color_stop_rgb"), FunctionTemplate::New(pattern_add_color_stop_rgb));
    cairo->Set(String::New("pattern_add_color_stop_rgba"), FunctionTemplate::New(pattern_add_color_stop_rgba));
    cairo->Set(String::New("pattern_get_stop_color_count"), FunctionTemplate::New(pattern_get_stop_color_count));
    cairo->Set(String::New("pattern_get_color_stop_rgba"), FunctionTemplate::New(pattern_get_color_stop_rgba));
    cairo->Set(String::New("pattern_create_rgb"), FunctionTemplate::New(pattern_create_rgb));
    cairo->Set(String::New("pattern_create_rgba"), FunctionTemplate::New(pattern_create_rgba));
    cairo->Set(String::New("pattern_get_rgba"), FunctionTemplate::New(pattern_get_rgba));
    cairo->Set(String::New("pattern_create_for_surface"), FunctionTemplate::New(pattern_create_for_surface));
    cairo->Set(String::New("pattern_get_surface"), FunctionTemplate::New(pattern_get_surface));
    cairo->Set(String::New("pattern_create_linear"), FunctionTemplate::New(pattern_create_linear));
    cairo->Set(String::New("pattern_get_linear_points"), FunctionTemplate::New(pattern_get_linear_points));
    cairo->Set(String::New("pattern_create_radial"), FunctionTemplate::New(pattern_create_radial));
    cairo->Set(String::New("pattern_get_radial_circles"), FunctionTemplate::New(pattern_get_radial_circles));
    cairo->Set(String::New("pattern_reference"), FunctionTemplate::New(pattern_reference));
    cairo->Set(String::New("pattern_status"), FunctionTemplate::New(pattern_status));
    cairo->Set(String::New("pattern_set_extend"), FunctionTemplate::New(pattern_set_extend));
    cairo->Set(String::New("pattern_get_extend"), FunctionTemplate::New(pattern_get_extend));
    cairo->Set(String::New("pattern_set_filter"), FunctionTemplate::New(pattern_set_filter));
    cairo->Set(String::New("pattern_get_filter"), FunctionTemplate::New(pattern_get_filter));
    cairo->Set(String::New("pattern_set_matrix"), FunctionTemplate::New(pattern_set_matrix));
    cairo->Set(String::New("pattern_get_matrix"), FunctionTemplate::New(pattern_get_matrix));
    cairo->Set(String::New("pattern_get_type"), FunctionTemplate::New(pattern_get_type));
    cairo->Set(String::New("pattern_get_reference_count"), FunctionTemplate::New(pattern_get_reference_count));
    cairo->Set(String::New("matrix_create"), FunctionTemplate::New(matrix_create));
    cairo->Set(String::New("matrix_init"), FunctionTemplate::New(matrix_init));
    cairo->Set(String::New("matrix_clone"), FunctionTemplate::New(matrix_clone));
    cairo->Set(String::New("matrix_init_identity"), FunctionTemplate::New(matrix_init_identity));
    cairo->Set(String::New("matrix_init_translate"), FunctionTemplate::New(matrix_init_translate));
    cairo->Set(String::New("matrix_init_scale"), FunctionTemplate::New(matrix_init_scale));
    cairo->Set(String::New("matrix_init_rotate"), FunctionTemplate::New(matrix_init_rotate));
    cairo->Set(String::New("matrix_translate"), FunctionTemplate::New(matrix_translate));
    cairo->Set(String::New("matrix_scale"), FunctionTemplate::New(matrix_scale));
    cairo->Set(String::New("matrix_rotate"), FunctionTemplate::New(matrix_rotate));
    cairo->Set(String::New("matrix_invert"), FunctionTemplate::New(matrix_invert));
    cairo->Set(String::New("matrix_multiply"), FunctionTemplate::New(matrix_multiply));
    cairo->Set(String::New("matrix_transform_distance"), FunctionTemplate::New(matrix_transform_distance));
    cairo->Set(String::New("matrix_transform_point"), FunctionTemplate::New(matrix_transform_point));
    cairo->Set(String::New("matrix_destroy"), FunctionTemplate::New(matrix_destroy));
#if CAIRO_VERSION_MINOR >= 10
    cairo->Set(String::New("region_create"), FunctionTemplate::New(region_create));
    cairo->Set(String::New("region_create_rectangle"), FunctionTemplate::New(region_create_rectangle));
    cairo->Set(String::New("region_create_rectangles"), FunctionTemplate::New(region_create_rectangles));
    cairo->Set(String::New("region_copy"), FunctionTemplate::New(region_copy));
    cairo->Set(String::New("region_reference"), FunctionTemplate::New(region_reference));
    cairo->Set(String::New("region_destroy"), FunctionTemplate::New(region_destroy));
    cairo->Set(String::New("region_status"), FunctionTemplate::New(region_status));
    cairo->Set(String::New("region_get_extents"), FunctionTemplate::New(region_get_extents));
    cairo->Set(String::New("region_num_rectangles"), FunctionTemplate::New(region_num_rectangles));
    cairo->Set(String::New("region_get_rectangle"), FunctionTemplate::New(region_get_rectangle));
    cairo->Set(String::New("region_is_empty"), FunctionTemplate::New(region_is_empty));
    cairo->Set(String::New("region_contains_point"), FunctionTemplate::New(region_contains_point));
    cairo->Set(String::New("region_contains_rectangle"), FunctionTemplate::New(region_contains_rectangle));
    cairo->Set(String::New("region_equal"), FunctionTemplate::New(region_equal));
    cairo->Set(String::New("region_translate"), FunctionTemplate::New(region_translate));
    cairo->Set(String::New("region_intersect"), FunctionTemplate::New(region_intersect));
    cairo->Set(String::New("region_intersect_rectangle"), FunctionTemplate::New(region_intersect_rectangle));
    cairo->Set(String::New("region_subtract"), FunctionTemplate::New(region_subtract));
    cairo->Set(String::New("region_subtract_rectangle"), FunctionTemplate::New(region_subtract_rectangle));
    cairo->Set(String::New("region_union"), FunctionTemplate::New(region_union));
    cairo->Set(String::New("region_union_rectangle"), FunctionTemplate::New(region_union_rectangle));
    cairo->Set(String::New("region_xor"), FunctionTemplate::New(region_xor));
    cairo->Set(String::New("region_xor_rectangle"), FunctionTemplate::New(region_xor_rectangle));
#endif    
    return cairo->NewInstance();
}
