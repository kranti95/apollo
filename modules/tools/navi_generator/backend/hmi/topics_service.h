/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/**
 * @file
 */

#ifndef MODULES_TOOLS_NAVI_GENERATOR_BACKEND_HMI_TOPICS_SERVICE_H_
#define MODULES_TOOLS_NAVI_GENERATOR_BACKEND_HMI_TOPICS_SERVICE_H_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "modules/common/log.h"
#include "modules/tools/navi_generator/backend/util/navigation_editor.h"
#include "modules/tools/navi_generator/backend/util/trajectory_collector.h"
#include "modules/tools/navi_generator/backend/util/trajectory_processor.h"
#include "modules/tools/navi_generator/backend/webserver/navi_generator_websocket.h"

namespace apollo {
namespace navi_generator {

/**
 * @class TopicsService
 * @brief
 * NOTE: This class is not thread-safe.
 */
class TopicsService {
 public:
  // The maximum number of monitor message items to be kept in
  // SimulationWorld.
  static constexpr int kMaxMonitorItems = 30;

  /**
   * @brief Constructor of TopicsService.
   * @param websocket Pointer of the websocket handler that has been attached to
   * the server.
   */
  explicit TopicsService(NaviGeneratorWebSocket *websocket);

  /**
   * @brief The function Update() is periodically called to check for updates
   * from the adapters.
   */
  void Update();

  // A callback function which updates the GUI.
  static void UpdateGUI(const std::string &msg, void *service);

  bool SetCommonBagFileInfo(
      const apollo::navi_generator::util::CommonBagFileInfo &common_file_info);
  bool ProcessBagFileSegment(
      const apollo::navi_generator::util::FileSegment &file_segment);
  bool SaveFilesToDatabase();

  bool StartCollector(const std::string &collection_type,
                      const std::size_t min_speed_limit,
                      const std::size_t max_speed_limit);
  bool StopCollector();
  bool UpdateCollector(const std::string &collection_type,
                       const std::size_t min_speed_limit,
                       const std::size_t max_speed_limit);
  bool GetCollectorOptions(const std::string &collection_type,
                           const std::size_t min_speed_limit,
                           const std::size_t max_speed_limit,
                           util::CollectorOptions *const options);

  nlohmann::json GetRoutePathAsJson(const nlohmann::json &map_data);

  bool CorrectRoadDeviation();
  bool SaveRoadCorrection();

  bool ModifySpeedLimit(const apollo::localization::msf::WGS84Corr &start_point,
                        const apollo::localization::msf::WGS84Corr &end_point,
                        const std::uint8_t new_speed_min,
                        const std::uint8_t new_speed_max);
  bool SaveSpeedLimit();

 private:
  // The pointer of NaviGeneratorWebSocket, not owned by TopicsService.
  NaviGeneratorWebSocket *websocket_ = nullptr;

  std::unique_ptr<util::TrajectoryProcessor> trajectory_processor_;
  std::unique_ptr<util::TrajectoryCollector> trajectory_collector_;
  std::unique_ptr<util::NavigationEditor> navigation_editor_;
};

}  // namespace navi_generator
}  // namespace apollo

#endif  // MODULES_TOOLS_NAVI_GENERATOR_BACKEND_HMI_TOPICS_SERVICE_H_
