#ifndef _RAYC_GAME_RAYCASTING_H_
#define _RAYC_GAME_RAYCASTING_H_ 1

#include <rayc/view.h>
#include <rayc/math/vec2.h>
#include <rayc/game/object.h>

#include <memory>
#include <string>
#include <list>

namespace rayc {

class Renderer : public View {
 private:
  enum Side {
    NORTH, SOUTH, WEST, EAST, TOP, BOTTOM
  };

  struct TileHit {
    Vec2i tilePosition {0, 0};
    Vec2d hitPosition {0, 0};
    float rayLength = 0.0f;
    float sampleX = 0.0f;
    Side side = NORTH;
  };

  struct DDAResult {
    bool hitWall = false;
    bool hitDoor = false;
    
    TileHit tile;
    TileHit door; // std::vector<TileHit> doors;
  };

 public:
  Renderer();
  ~Renderer();

  bool isRunning() const;

  void onFrameUpdate(float frameTime) override;
  void onEvent(const std::string& eventName, std::map<std::string, std::string> args) override;

 private:
  void run(const std::string& map);
  void clear();

  DDAResult castRay(Vec2d src, Vec2d direction);
  void render(float frameTime);
  void processInput(float frameTime);

 private:
  float mFov = 3.14159f;
  float mDepth = 30.0f;
  float mStep = 0.01f;
  int mTextureColumnWidth = 1;
  float* mDepthBuffer = nullptr;

  std::list<std::pair<int, Object*>> mObjects;

  bool mIsRunning = false;
  std::string mMapName;
};

} /* namespace rayc */

#endif /* _RAYC_GAME_RAYCASTING_H_ */