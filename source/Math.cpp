#include "Math.h"
#include "Vector2f.h"
#include <cmath>

bool checkCollision(Box box, Circle circle) {
  // Calculate the closest point to the circle within the rectangle
  float closestX =
      std::max(box.getLeft(), std::min(circle.position.x, box.getRight()));
  float closestY =
      std::max(box.getTop(), std::min(circle.position.y, box.getBottom()));

  // Calculate the distance between the circle's center and this closest point
  float distanceX = circle.position.x - closestX;
  float distanceY = circle.position.y - closestY;
  float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

  // Check if the distance is less than the circle's radius squared
  return distanceSquared < (circle.radius * circle.radius);
}

std::string floatToString(float value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

#include "GameManager.h"

Vector2f getLogcialPosition(Vector2f screenPos) {
  int virtualWidth, virtualHeight;
  SDL_RenderGetLogicalSize(GameManager::renderer, &virtualWidth,
                           &virtualHeight);

  int windowWidth, windowHeight;
  SDL_GetWindowSize(GameManager::window, &windowWidth, &windowHeight);

  double scale;
  int xOffset, yOffset;
  if (windowWidth * virtualHeight > windowHeight * virtualWidth) {
    // Calculate the scale based on height
    scale = (double)windowHeight / virtualHeight;
    xOffset = (windowWidth - virtualWidth * scale) / 2;
    yOffset = 0;
  } else {
    // Calculate the scale based on width
    scale = (double)windowWidth / virtualWidth;
    xOffset = 0;
    yOffset = (windowHeight - virtualHeight * scale) / 2;
  }

  float logicalX = 1.0f * (screenPos.x - xOffset) / scale;
  float logicalY = 1.0f * (screenPos.y - yOffset) / scale;

  return {logicalX, logicalY};
}

Vector2f getScreenPosition(Vector2f logicalPos) {
  int virtualWidth, virtualHeight;
  SDL_RenderGetLogicalSize(GameManager::renderer, &virtualWidth,
                           &virtualHeight);

  int windowWidth, windowHeight;
  SDL_GetWindowSize(GameManager::window, &windowWidth, &windowHeight);

  double scale;
  int xOffset, yOffset;
  if (windowWidth * virtualHeight > windowHeight * virtualWidth) {
    // Calculate the scale based on height
    scale = (double)windowHeight / virtualHeight;
    xOffset = (windowWidth - virtualWidth * scale) / 2;
    yOffset = 0;
  } else {
    // Calculate the scale based on width
    scale = (double)windowWidth / virtualWidth;
    xOffset = 0;
    yOffset = (windowHeight - virtualHeight * scale) / 2;
  }

  float screenX = logicalPos.x * scale + xOffset;
  float screenY = logicalPos.y * scale + yOffset;

  return {screenX, screenY};
}

Vector2f getImGuiPosition(Vector2f pos) {
  Vector2f out;

  SDL_Rect logicalRect = UpdateLogicalPresentation(GameManager::renderer);
  Vector2f logicalPos = {(float)logicalRect.x, (float)logicalRect.y};

  out = pos + GameManager::gameWindowSize / 2;
  return out;
}

SDL_Rect UpdateLogicalPresentation(SDL_Renderer *renderer) {
  int logical_w = 1, logical_h = 1;
  // int logical_w = GameManager::gameWindowSize.x,
  //     logical_h = GameManager::gameWindowSize.y;
  int output_w = GameManager::currentWindowSize.x;
  int output_h = GameManager::currentWindowSize.y;
  float want_aspect = 1.0f;
  float real_aspect = 1.0f;
  float scale;
  SDL_Rect logical_src_rect;
  SDL_Rect logical_dst_rect;

  if (false) {
    /* All done! */
  }

  // if (SDL_QueryTexture(renderer->logical_target, NULL, NULL, &logical_w,
  // &logical_h) < 0) {
  //     goto error;
  // }

  want_aspect = (float)logical_w / logical_h;
  real_aspect = (float)output_w / output_h;

  logical_src_rect.x = 0.0f;
  logical_src_rect.y = 0.0f;
  logical_src_rect.w = (float)logical_w;
  logical_src_rect.h = (float)logical_h;

  // if (false) {
  //   if (want_aspect > real_aspect) {
  //     scale = (float)(output_w / logical_w); /* This an integer division! */
  //   } else {
  //     scale = (float)(output_h / logical_h); /* This an integer division! */
  //   }
  //
  //   if (scale < 1.0f) {
  //     scale = 1.0f;
  //   }
  //
  //   logical_dst_rect.w = SDL_floorf(logical_w * scale);
  //   logical_dst_rect.x = (output_w - logical_dst_rect.w) / 2.0f;
  //   logical_dst_rect.h = SDL_floorf(logical_h * scale);
  //   logical_dst_rect.y = (output_h - logical_dst_rect.h) / 2.0f;
  //
  // } else if (false || SDL_fabsf(want_aspect - real_aspect) < 0.0001f) {
  //   logical_dst_rect.x = 0.0f;
  //   logical_dst_rect.y = 0.0f;
  //   logical_dst_rect.w = (float)output_w;
  //   logical_dst_rect.h = (float)output_h;
  //
  // }
  if (want_aspect > real_aspect) {
    if (true) {
      /* We want a wider aspect ratio than is available - letterbox it */
      scale = (float)output_w / logical_w;
      logical_dst_rect.x = 0.0f;
      logical_dst_rect.w = (float)output_w;
      logical_dst_rect.h = SDL_floorf(logical_h * scale);
      logical_dst_rect.y = (output_h - logical_dst_rect.h) / 2.0f;
    } else { /* renderer->logical_presentation_mode ==
                SDL_LOGICAL_PRESENTATION_OVERSCAN */
      /* We want a wider aspect ratio than is available -
         zoom so logical height matches the real height
         and the width will grow off the screen
       */
      scale = (float)output_h / logical_h;
      logical_dst_rect.y = 0.0f;
      logical_dst_rect.h = (float)output_h;
      logical_dst_rect.w = SDL_floorf(logical_w * scale);
      logical_dst_rect.x = (output_w - logical_dst_rect.w) / 2.0f;
    }
  } else {
    if (true) {
      /* We want a narrower aspect ratio than is available - use side-bars */
      scale = (float)output_h / logical_h;
      logical_dst_rect.y = 0.0f;
      logical_dst_rect.h = (float)output_h;
      logical_dst_rect.w = SDL_floorf(logical_w * scale);
      logical_dst_rect.x = (output_w - logical_dst_rect.w) / 2.0f;
    } else { /* renderer->logical_presentation_mode ==
                SDL_LOGICAL_PRESENTATION_OVERSCAN */
      /* We want a narrower aspect ratio than is available -
         zoom so logical width matches the real width
         and the height will grow off the screen
       */
      scale = (float)output_w / logical_w;
      logical_dst_rect.x = 0.0f;
      logical_dst_rect.w = (float)output_w;
      logical_dst_rect.h = SDL_floorf(logical_h * scale);
      logical_dst_rect.y = (output_h - logical_dst_rect.h) / 2.0f;
    }
  }

  // SDL_SetTextureScaleMode(renderer->logical_target,
  // renderer->logical_scale_mode);
  //
  // if (!renderer->target) {
  //     SDL_SetRenderTarget(renderer, renderer->logical_target);
  // }

  return logical_dst_rect;

  // error:
  //     SDL_SetRenderLogicalPresentation(renderer, 0, 0,
  //     SDL_LOGICAL_PRESENTATION_DISABLED, SDL_SCALEMODE_NEAREST);
  // return -1;
}

// static int UpdateLogicalPresentation(SDL_Renderer *renderer)
// {
//     int logical_w = 1, logical_h = 1;
//     int output_w = renderer->main_view.pixel_w;
//     int output_h = renderer->main_view.pixel_h;
//     float want_aspect = 1.0f;
//     float real_aspect = 1.0f;
//     float scale;
//
//     if (renderer->logical_presentation_mode ==
//     SDL_LOGICAL_PRESENTATION_DISABLED) {
//         /* All done! */
//         return 0;
//     }
//
//     if (SDL_QueryTexture(renderer->logical_target, NULL, NULL, &logical_w,
//     &logical_h) < 0) {
//         goto error;
//     }
//
//     want_aspect = (float)logical_w / logical_h;
//     real_aspect = (float)output_w / output_h;
//
//     renderer->logical_src_rect.x = 0.0f;
//     renderer->logical_src_rect.y = 0.0f;
//     renderer->logical_src_rect.w = (float)logical_w;
//     renderer->logical_src_rect.h = (float)logical_h;
//
//     if (renderer->logical_presentation_mode ==
//     SDL_LOGICAL_PRESENTATION_INTEGER_SCALE) {
//         if (want_aspect > real_aspect) {
//             scale = (float)(output_w / logical_w); /* This an integer
//             division! */
//         } else {
//             scale = (float)(output_h / logical_h); /* This an integer
//             division! */
//         }
//
//         if (scale < 1.0f) {
//             scale = 1.0f;
//         }
//
//         renderer->logical_dst_rect.w = SDL_floorf(logical_w * scale);
//         renderer->logical_dst_rect.x = (output_w -
//         renderer->logical_dst_rect.w) / 2.0f; renderer->logical_dst_rect.h =
//         SDL_floorf(logical_h * scale); renderer->logical_dst_rect.y =
//         (output_h - renderer->logical_dst_rect.h) / 2.0f;
//
//     } else if (renderer->logical_presentation_mode ==
//     SDL_LOGICAL_PRESENTATION_STRETCH ||
//                SDL_fabsf(want_aspect - real_aspect) < 0.0001f) {
//         renderer->logical_dst_rect.x = 0.0f;
//         renderer->logical_dst_rect.y = 0.0f;
//         renderer->logical_dst_rect.w = (float)output_w;
//         renderer->logical_dst_rect.h = (float)output_h;
//
//     } else if (want_aspect > real_aspect) {
//         if (renderer->logical_presentation_mode ==
//         SDL_LOGICAL_PRESENTATION_LETTERBOX) {
//             /* We want a wider aspect ratio than is available - letterbox it
//             */ scale = (float)output_w / logical_w;
//             renderer->logical_dst_rect.x = 0.0f;
//             renderer->logical_dst_rect.w = (float)output_w;
//             renderer->logical_dst_rect.h = SDL_floorf(logical_h * scale);
//             renderer->logical_dst_rect.y = (output_h -
//             renderer->logical_dst_rect.h) / 2.0f;
//         } else { /* renderer->logical_presentation_mode ==
//         SDL_LOGICAL_PRESENTATION_OVERSCAN */
//             /* We want a wider aspect ratio than is available -
//                zoom so logical height matches the real height
//                and the width will grow off the screen
//              */
//             scale = (float)output_h / logical_h;
//             renderer->logical_dst_rect.y = 0.0f;
//             renderer->logical_dst_rect.h = (float)output_h;
//             renderer->logical_dst_rect.w = SDL_floorf(logical_w * scale);
//             renderer->logical_dst_rect.x = (output_w -
//             renderer->logical_dst_rect.w) / 2.0f;
//         }
//     } else {
//         if (renderer->logical_presentation_mode ==
//         SDL_LOGICAL_PRESENTATION_LETTERBOX) {
//             /* We want a narrower aspect ratio than is available - use
//             side-bars */ scale = (float)output_h / logical_h;
//             renderer->logical_dst_rect.y = 0.0f;
//             renderer->logical_dst_rect.h = (float)output_h;
//             renderer->logical_dst_rect.w = SDL_floorf(logical_w * scale);
//             renderer->logical_dst_rect.x = (output_w -
//             renderer->logical_dst_rect.w) / 2.0f;
//         } else { /* renderer->logical_presentation_mode ==
//         SDL_LOGICAL_PRESENTATION_OVERSCAN */
//             /* We want a narrower aspect ratio than is available -
//                zoom so logical width matches the real width
//                and the height will grow off the screen
//              */
//             scale = (float)output_w / logical_w;
//             renderer->logical_dst_rect.x = 0.0f;
//             renderer->logical_dst_rect.w = (float)output_w;
//             renderer->logical_dst_rect.h = SDL_floorf(logical_h * scale);
//             renderer->logical_dst_rect.y = (output_h -
//             renderer->logical_dst_rect.h) / 2.0f;
//         }
//     }
//
//     SDL_SetTextureScaleMode(renderer->logical_target,
//     renderer->logical_scale_mode);
//
//     if (!renderer->target) {
//         SDL_SetRenderTarget(renderer, renderer->logical_target);
//     }
//
//     return 0;
//
// error:
//     SDL_SetRenderLogicalPresentation(renderer, 0, 0,
//     SDL_LOGICAL_PRESENTATION_DISABLED, SDL_SCALEMODE_NEAREST); return -1;
// }
