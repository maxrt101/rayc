#include <rayc/game/raycasting.h>
#include <rayc/game/state.h>
#include <rayc/renderer/utils.h>
#include <rayc/data/resources.h>
#include <rayc/app.h>
#include <rayc/log.h>

#include <chrono>

rayc::Renderer::Renderer() : View("renderer") {
  mDepthBuffer = new float[app::getWidth()];
  // mFov = app::getWidth() / (app::getHeight() * 2);
  mFov = M_PI / 4.8f;
  // mFov = M_PI / 2.0f;
}

rayc::Renderer::~Renderer() {
  delete [] mDepthBuffer;
}

bool rayc::Renderer::isRunning() const {
  return mIsRunning;
}

void rayc::Renderer::onFrameUpdate(float frameTime) {
  if (mIsRunning) {
    processInput(frameTime);
    render(frameTime);
  }
}

void rayc::Renderer::onEvent(const std::string& eventName, std::map<std::string, std::string> args) {
  if (eventName == "run") {
    // if (args.find("map") != args.end()) {
    //   mMapName = args["map"];
    // }
    // if (mMapName.size()) {
    //   getPlayer().position = {
    //     (float) getMap(mMapName)->startX,
    //     (float) getMap(mMapName)->startY
    //   };
    //   getPlayer().health = 100;
    //   getPlayer().weapons.clear();
    //   // getPlayer().weapons.push_back(); // TODO:
    //   mIsRunning = true;
    // } else {
    //   error("(renderer) Can't run if no map is selected");
    // }
    run(args["map"]);
  } else if (eventName == "pause") {
    mIsRunning = false;
  } else if (eventName == "setMap") {
    if (args.find("map") != args.end()) {
      mMapName = args["map"];
    } else {
      error("(renderer) Missing required event argument 'map'");
    }
  } else {
    error("(renderer) Unsupported event '%s'", eventName.c_str());
  }
}

void rayc::Renderer::run(const std::string& map) {
  clear();

  if (!map.empty()) {
    mMapName = map;
  }

  if (mMapName.size()) {
    if (!getMap(mMapName)) {
      mMapName.clear();
      error("(renderer) Invalid map");
      return;
    }

    getPlayer().position = {
      (float) getMap(mMapName)->startX,
      (float) getMap(mMapName)->startY
    };
    getPlayer().health = 100;
    getPlayer().weapons.clear();
    // getPlayer().weapons.push_back(); // TODO:

    for (auto& e : getMap(mMapName)->entities) {
      auto object = Object::fromEntity(getEntity(getMap(mMapName)->entityTypeTranslations[e.type]));
      object->position.x = e.x;
      object->position.y = e.y;
      mObjects.push_back({0, object});
    }

    mIsRunning = true;
  } else {
    error("(renderer) Can't run if no map is selected");
  }
}

void rayc::Renderer::clear() {
  for (auto& [_, object] : mObjects) {
    delete object;
  }
  mObjects.clear();

  for (int i = 0; i < app::getWidth(); i++) {
    mDepthBuffer[i] = 0;
  }
}

rayc::Renderer::DDAResult rayc::Renderer::castRay(Vec2d src, Vec2d direction) {
  DDAResult result;

  Vec2d rayDelta = {
    sqrt(1 + (direction.y / direction.x) * (direction.y / direction.x)),
    sqrt(1 + (direction.x / direction.y) * (direction.x / direction.y))
  };

  Vec2i mapCheck = {(int)src.x, (int)src.y};
  Vec2d sideDistance, stepDistance;

  if (direction.x < 0) {
    stepDistance.x = -1;
    sideDistance.x = (src.x - (float)mapCheck.x) * rayDelta.x;
  } else {
    stepDistance.x = 1;
    sideDistance.x = ((float)mapCheck.x + 1 - src.x) * rayDelta.x;
  }

  if (direction.y < 0) {
    stepDistance.y = -1;
    sideDistance.y = (src.y - (float)mapCheck.y) * rayDelta.y;
  } else {
    stepDistance.y = 1;
    sideDistance.y = ((float)mapCheck.y + 1 - src.y) * rayDelta.y;
  }

  Vec2d intersection;
  Vec2i hitTile;
  float maxDistance = 100.0f;
  float distance = 0.0f;
  TileHit hit;

  while (!result.hitWall && distance < maxDistance) {
    if (sideDistance.x < sideDistance.y) {
      sideDistance.x += rayDelta.x;
      mapCheck.x += stepDistance.x;
    } else {
      sideDistance.y += rayDelta.y;
      mapCheck.y += stepDistance.y;
    }

    Vec2d rayDistance = {(float)mapCheck.x - src.x, (float)mapCheck.y - src.y};
    distance = sqrt(rayDistance.x * rayDistance.x + rayDistance.y * rayDistance.y);

    if (getMap(mMapName)->getTile(mapCheck).isSolid()) {
      hitTile = mapCheck;

      /*if (res.map.getTile(mapCheck).isDoor() && res.map.getTile(mapCheck).doorState != DOOR_CLOSED) {
        // tileFound = false;
        result.hitDoor = true;
      } else {
        // tileFound = true;
        result.hitWall = true;
      }*/
      result.hitWall = true;

      hit.tilePosition = mapCheck;

      float m = direction.y / direction.x;

      if (src.y <= mapCheck.y) {
        if (src.x <= mapCheck.x) {
          hit.side = WEST;
          intersection.y = m * (mapCheck.x - src.x) + src.y;
          intersection.x = float(mapCheck.x);
          hit.sampleX = intersection.y - std::floor(intersection.y);
        } else if (src.x >= (mapCheck.x + 1)) {
          hit.side = EAST;
          intersection.y = m * ((mapCheck.x + 1) - src.x) + src.y;
          intersection.x = float(mapCheck.x + 1);
          hit.sampleX = intersection.y - std::floor(intersection.y);
        } else {
          hit.side = NORTH;
          intersection.y = float(mapCheck.y);
          intersection.x = (mapCheck.y - src.y) / m + src.x;
          hit.sampleX = intersection.x - std::floor(intersection.x);
        }

        if (intersection.y < mapCheck.y) {
          hit.side = NORTH;
          intersection.y = float(mapCheck.y);
          intersection.x = (mapCheck.y - src.y) / m + src.x;
          hit.sampleX = intersection.x - std::floor(intersection.x);
        }
      } else if (src.y >= mapCheck.y + 1) {
        if (src.x <= mapCheck.x) {
          hit.side = WEST;
          intersection.y = m * (mapCheck.x - src.x) + src.y;
          intersection.x = float(mapCheck.x);
          hit.sampleX = intersection.y - std::floor(intersection.y);
        } else if (src.x >= (mapCheck.x + 1)) {
          hit.side = EAST;
          intersection.y = m * ((mapCheck.x + 1) - src.x) + src.y;
          intersection.x = float(mapCheck.x + 1);
          hit.sampleX = intersection.y - std::floor(intersection.y);
        } else {
          hit.side = SOUTH;
          intersection.y = float(mapCheck.y + 1);
          intersection.x = ((mapCheck.y + 1) - src.y) / m + src.x;
          hit.sampleX = intersection.x - std::floor(intersection.x);
        }

        if (intersection.y > (mapCheck.y + 1)) {
          hit.side = SOUTH;
          intersection.y = float(mapCheck.y + 1);
          intersection.x = ((mapCheck.y + 1) - src.y) / m + src.x;
          hit.sampleX = intersection.x - std::floor(intersection.x);
        }
      } else {
        if (src.x <= mapCheck.x) {
          hit.side = WEST;
          intersection.y = m * (mapCheck.x - src.x) + src.y;
          intersection.x = float(mapCheck.x);
          hit.sampleX = intersection.y - std::floor(intersection.y);
        } else if (src.x >= (mapCheck.x + 1)) {
          hit.side = EAST;
          intersection.y = m * ((mapCheck.x + 1) - src.x) + src.y;
          intersection.x = float(mapCheck.x + 1);
          hit.sampleX = intersection.y - std::floor(intersection.y);
        }
      }

      hit.hitPosition = intersection;

      result.tile = hit;
      /*if (res.map.getTile(mapCheck).isDoor() && res.map.getTile(mapCheck).doorState != DOOR_CLOSED) {
        result.door = hit;
      } else {
        result.tile = hit;
      }*/
    }
  }

  return result;
}

void rayc::Renderer::render(float frameTime) {
  auto start = std::chrono::system_clock::now();

  int screenHeight = app::getHeight();
  int screenWidth = app::getWidth();
  int mapHeight = getMap(mMapName)->height;
  int mapWidth = getMap(mMapName)->width;

  setDrawColor(128, 128, 128, 255);
  fillRect({0, screenHeight/2, screenWidth, screenHeight/2});

  auto wallRenderStart = std::chrono::system_clock::now();

  for (int x = 0; x < screenWidth; x++) {
    float rayAngle = (getPlayer().angle - mFov/2.0f) + (x / (float)screenWidth) * mFov;
    Vec2d rayDirection = {sinf(rayAngle), cosf(rayAngle)};

    DDAResult result = castRay(getPlayer().position, rayDirection);

    if (result.hitWall) {
      Vec2d ray = result.tile.hitPosition - getPlayer().position;
      float rayLength = sqrt(ray.x * ray.x + ray.y * ray.y) * cos(rayAngle - getPlayer().angle);

      mDepthBuffer[x] = rayLength;

      float ceiling = (screenHeight / 2.0f) - screenHeight / rayLength;
      float floor = screenHeight - ceiling;
      float wallHeight = floor - ceiling;

      int textureIdx = getMap(mMapName)->getTile(result.tile.tilePosition).texture;
      // Texture* texture = getTexture(""); //&res.texturePlaceholder;
      // if (textureIdx >= 0 && textureIdx < res.textures.size()) {
      //   texture = &res.textures[textureIdx];
      // }
      Texture* texture = getTexture(getMap(mMapName)->textureTranslations[textureIdx]);

      float whole;
      int textureX = std::modf(result.tile.sampleX, &whole) * texture->getWidth();

      if (result.tile.side == SOUTH || result.tile.side == WEST) {
        textureX = texture->getWidth() - textureX - 1;
      }

      copyTexture(texture, {textureX, 0, 1, texture->getHeight()}, {x, (int) ceiling, 1, (int) wallHeight});
    }

  }

  auto wallRenderEnd = std::chrono::system_clock::now();

  // Objects
  auto objectRenderStart = std::chrono::system_clock::now();

  for (auto& [distance, object] : mObjects) {
    if (!object || !object->entity) {
      // TODO: error
      continue;
    }

    // object->onFrameUpdate(frameTime); // TODO: update position using velocity
    
    // if (getMap(mMapName)->getTile(object->position.x, object->position.y).isSolid()) {
      // object->onCollision(nullptr); // TODO: handle collision
    // }

    Vec2d vec = object->position - getPlayer().position;
    Vec2d direction = {sinf(getPlayer().angle), cosf(getPlayer().angle)};

    float objectAngle = atan2f(direction.y, direction.x) - atan2f(vec.y, vec.x);
    float distanceFromPlayer = sqrtf(vec.x*vec.x + vec.y*vec.y) * cosf(objectAngle);
    distance = distanceFromPlayer;

    if (objectAngle < -M_PI) {
      objectAngle += 2.0f * M_PI;
    }

    if (objectAngle > M_PI) {
      objectAngle -= 2.0f * M_PI;
    }

    /*bool isInFov = fabs(objectAngle) < mFov / 2.0f;

    if (isInFov && distanceFromPlayer >= 0.5 && distanceFromPlayer < mDepth) {
      float objectCeiling = (float)(screenHeight / 2.0) - screenHeight / (float) distanceFromPlayer;
      float objectFloor = screenHeight - objectCeiling;
      float objectHeight = objectFloor - objectCeiling;
      float middleOfObject = (0.5f * (objectAngle / (mFov * 0.5f)) + 0.5f) * screenWidth;

      auto texture = getTexture(object->getCurrentTexture());

      if (!texture) {
        error("(renderer) No texture found (texture=%s entity=%s)", object->getCurrentTexture().c_str(), object->entity->tag.c_str());
        continue;
      }

      for (int sx = 0; sx < texture->getWidth(); sx++) {
        int textureX = (sx / texture->getWidth());

        int objectColumn = (int)(middleOfObject + sx - (texture->getWidth() / 2.0f));

        // if (mDepthBuffer[objectColumn] >= distanceFromPlayer) {
          copyTexture(texture,
            {textureX, 0, 1, texture->getHeight()},
            {objectColumn, (int) objectCeiling, 1, (int) texture->getHeight()}
          );
          // mDepthBuffer[objectColumn] = distanceFromPlayer;
        // }
      }
    }*/

    bool isInFov = fabs(objectAngle) < (mFov + (1.0f / distanceFromPlayer)) / 2.0f;

    if (isInFov && distanceFromPlayer >= 0.5f && distanceFromPlayer < mDepth) {
      Vec2d floorPoint = {
        (0.5f * ((objectAngle / (mFov * 0.5f))) + 0.5f) * screenWidth,
        (screenHeight / 2.0f) + (screenHeight / distanceFromPlayer) / std::cos(objectAngle / 2.0f)
      };

      auto texture = getTexture(object->getCurrentTexture());

      if (!texture) {
        error("(renderer) No texture found (texture=%s entity=%s)", object->getCurrentTexture().c_str(), object->entity->tag.c_str());
        continue;
      }

      Vec2d objectSize = {(double) texture->getWidth(), (double) texture->getHeight()};
      objectSize *= 2.0f * screenHeight / objectSize.y;
      objectSize /= distanceFromPlayer;

      Vec2i start = {(int)(floorPoint.x - objectSize.x / 2.0f), (int)(floorPoint.y - objectSize.y + 100.0f/distanceFromPlayer)};

      // float whole;

      // printf("obj: sz=(%f %f) a=%f d=%f st=(%d %d)\n", objectSize.x, objectSize.y, objectAngle, distanceFromPlayer, start.x, start.y);

      for (int sx = 0; sx < objectSize.x; sx++) {
        int textureX = (sx / objectSize.x) * texture->getWidth();

        if (mDepthBuffer[start.x + sx] >= distanceFromPlayer) {
          copyTexture(texture,
            {textureX, 0, 1, texture->getHeight()},
            {start.x+sx, start.y, 1, (int)objectSize.y}
          );

          // if (spriteOverlay) {
          //   copyTexture(&res.textureOverlay,
          //     {textureX, 0, 1, object->texture->getHeight()},
          //     {start.x+sx, start.y, 1, (int)objectSize.y}
          //   );
          // }
        }
      }
    }

    mObjects.remove_if([](auto& p) { return p.second->remove; });
    mObjects.sort([](const auto& a, const auto& b) { return a.first > b.first; });
  }
}

void rayc::Renderer::processInput(float frameTime) {
  if (app::getKeyState(SDL_SCANCODE_LEFT).held) {
    getPlayer().angle -= getPlayer().rotationSpeed * frameTime;
    if (getPlayer().angle < -2.0f * M_PI) { // kinda works?
      getPlayer().angle += 2.0f * M_PI;
    }
  }

  if (app::getKeyState(SDL_SCANCODE_RIGHT).held) {
    getPlayer().angle += getPlayer().rotationSpeed * frameTime;
    if (getPlayer().angle > 2.0f * M_PI) { // kinda works?
      getPlayer().angle -= 2.0f * M_PI;
    }
  }

  if (app::getKeyState(SDL_SCANCODE_W).held) {
    getPlayer().position.x += sinf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
    getPlayer().position.y += cosf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;

    if (getMap(mMapName)->getTile((int) getPlayer().position.x, (int) getPlayer().position.y).isSolid()) {
      getPlayer().position.x -= sinf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
      getPlayer().position.y -= cosf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
    }
  }

  if (app::getKeyState(SDL_SCANCODE_S).held) {
    getPlayer().position.x -= sinf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
    getPlayer().position.y -= cosf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;

    if (getMap(mMapName)->getTile((int) getPlayer().position.x, (int) getPlayer().position.y).isSolid()) {
      getPlayer().position.x += sinf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
      getPlayer().position.y += cosf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
    }
  }

  if (app::getKeyState(SDL_SCANCODE_A).held) {
    getPlayer().position.x -= cosf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
    getPlayer().position.y += sinf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;

    if (getMap(mMapName)->getTile((int) getPlayer().position.x, (int) getPlayer().position.y).isSolid()) {
      getPlayer().position.x += cosf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
      getPlayer().position.y -= sinf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
    }
  }
  
  if (app::getKeyState(SDL_SCANCODE_D).held) {
    getPlayer().position.x += cosf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
    getPlayer().position.y -= sinf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;

    if (getMap(mMapName)->getTile((int) getPlayer().position.x, (int) getPlayer().position.y).isSolid()) {
      getPlayer().position.x -= cosf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
      getPlayer().position.y += sinf(getPlayer().angle) * getPlayer().movementSpeed * frameTime;
    }
  }
}
