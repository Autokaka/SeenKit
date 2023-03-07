/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

namespace seen::framework::g2d::core {

// export interface IGenerateTextureOptions
//{
//   /** The scale mode of the texture. Optional, defaults to `PIXI.BaseTexture.defaultOptions.scaleMode`. */
//   scaleMode?: SCALE_MODES;
//   /** The resolution / device pixel ratio of the texture being generated. Optional defaults to Renderer resolution.
//   */ resolution?: number;
//   /**
//      * The region of the displayObject, that shall be rendered,
//      * if no region is specified, defaults to the local bounds of the displayObject.
//    */
//   region?: Rectangle;
//   /** The number of samples of the frame buffer. */
//   multisample?: MSAA_QUALITY;
// }

struct IGenerateTextureOptions {};

}  // namespace seen::framework::g2d::core