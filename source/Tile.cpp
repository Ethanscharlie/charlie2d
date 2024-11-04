// #include "Tile.hpp"
// #include "Box.hpp"
// #include "GameManager.hpp"
// #include "SDL_render.h"
// #include <memory>
//
// /*
//  * TILESET JSON EX
//  * "__cWid": 4,
//  * "__cHei": 2,
//  * "identifier": "Temp_basic_tiles",
//  * "uid": 1,
//  * "relPath": "images/temp_basic_tiles.png",
//  * "embedAtlas": null,
//  * "pxWid": 64,
//  * "pxHei": 32,
//  * "tileGridSize": 16,
//  * "spacing": 0,
//  * "padding": 0,
//  * "tags": [],
//  * "tagsSourceEnumUid": null,
//  * "enumTags": [],
//  * "customData": [],
//  * "savedSelections": [],
//  */
// Tileset::Tileset(const json &data, std::filesystem::path jsonDir) {
//   int tileGridSize = data["tileGridSize"];
//   width = data["__cWid"];
//   height = data["__cHei"];
// 	identifier = data["__identifier"];
// 	type = data["__type"];
// 	gridSize = data["__gridSize"];
// 	opacity = data["__opacity"];
// 	pxTotalOffsetX = data["__pxTotalOffsetX"];
// 	pxTotalOffsetY = data["__pxTotalOffsetY"];
// 	tilesetDefUid = data["__tilesetDefUid"];
// 	tilesetRelPath = data["__tilesetRelPath"];
// 	iid = data["iid"];
// 	levelId = data["levelId"];
// 	layerDefUid = data["layerDefUid"];
// 	pxOffsetX = data["pxOffsetX"];
// 	pxOffsetY = data["pxOffsetY"];
// 	visible = data["visible"];
// 	seed = data["seed"];
//
//   std::filesystem::path imageFileLocation;
//   imageFileLocation = std::filesystem::path(jsonDir) / data["relPath"];
//
//   for (int y = 0; y < height; y++) {
//     for (int x = 0; x < height; x++) {
//       Image image = {imageFileLocation.string()};
//       image.srcRect.x = x * tileGridSize;
//       image.srcRect.y = y * tileGridSize;
//       dictionary[TileLoc(x, y)] = image;
//     }
//   }
// }
//
// AllTilesets Tileset::getAllTilesets(const json &rootldtk, std::filesystem::path jsonDir) {
//   AllTilesets allTilesets; 
//   const json& tilesetsJson = rootldtk["defs"]["tilesets"];
//
//   for (auto& tilesetJson : tilesetsJson) {
//     allTilesets[tilesetJson["uid"]] = Tileset(tilesetJson, jsonDir);
//   }
//   return allTilesets;
// }
//
// /*
//  * "__identifier": "Background",
//  * "__type": "AutoLayer",
//  * "__cWid": 70,
//  * "__cHei": 20,
//  * "__gridSize": 16,
//  * "__opacity": 1,
//  * "__pxTotalOffsetX": 0,
//  * "__pxTotalOffsetY": 0,
//  * "__tilesetDefUid": 1,
//  * "__tilesetRelPath": "images/temp_basic_tiles.png",
//  * "iid": "b62181b0-4ce0-11ef-952f-d3c73fdaee0f",
//  * "levelId": 0,
//  * "layerDefUid": 92,
//  * "pxOffsetX": 0,
//  * "pxOffsetY": 0,
//  * "visible": true,
//  * "optionalRules": [],
//  * "intGridCsv": [],
//  * "autoLayerTiles": [
//  * 	{ "px": [48,0], "src": [16,16], "f": 0, "t": 5, "d": [99,3], "a": 1 },
//  * 	{ "px": [64,0], "src": [16,16], "f": 0, "t": 5, "d": [99,4], "a": 1 },
//  * 	{ "px": [80,0], "src": [16,16], "f": 0, "t": 5, "d": [99,5], "a": 1 },
//  */
// TileLayer::TileLayer(const json &data, const AllTilesets &allTilesets) {
//   if (!data["visible"])
//     return;
//   if (data["__type"] == "AutoLayer" || data["__type"] == "TileLayer")
//     return;
//
//   gridSize = data["__gridSize"];
//   int tilesetUid = data["__tilesetDefUid"];
//   tileset = std::addressof(allTilesets.at(tilesetUid));
//
//   width = data["__cWid"];
//   height = data["__cHei"];
//
//   json tilesList;
//   if (data["__type"] == "Tiles") {
//     tilesList = data["gridTiles"];
//   } else if (data["__type"] == "AutoLayer") {
//     tilesList = data["autoLayerTiles"];
//   }
//
//   for (const json &tileJson : tilesList) {
//     TileLoc tilesetTile = {(int)tileJson["src"][0] / gridSize,
//                            (int)tileJson["src"][1] / gridSize};
//
//     grid.push_back(tilesetTile);
//   }
// }
//
// void TileLayer::render() {
//   if (texture != nullptr) {
//     SDL_DestroyTexture(texture);
//   }
//
//   texture = SDL_CreateTexture(GameManager::renderer, SDL_PIXELFORMAT_RGBA8888,
//                               SDL_TEXTUREACCESS_TARGET, width * gridSize,
//                               height * gridSize);
//
//   SDL_SetRenderTarget(GameManager::renderer, texture);
//
//   int pointerX = 0;
//   int pointerY = 0;
//   for (TileLoc tileLoc : grid) {
//     const Image &image = tileset->dictionary.at(tileLoc);
//
//     SDL_Rect dstrect;
//     dstrect.x = pointerX * gridSize;
//     dstrect.y = pointerY * gridSize;
//     dstrect.w = gridSize;
//     dstrect.h = gridSize;
//
//     boxes.push_back(Box(dstrect.x, dstrect.y, gridSize, gridSize));
//
//     SDL_RenderCopy(GameManager::renderer, image.texture, &image.srcRect,
//                    &dstrect);
//
//     pointerX++;
//     if (pointerX > width) {
//       pointerX = 0;
//       pointerY++;
//     }
//   }
//
//   SDL_SetRenderTarget(GameManager::renderer, nullptr);
//   return {texture};
// }
//
// TileLayer::~TileLayer() {
//   if (texture != nullptr) {
//     SDL_DestroyTexture(texture);
//   }
// }
