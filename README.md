# haya_imu_ros2

<img src="https://github.com/soarbear/haya_imu_ros2/blob/main/image/haya_imu(icm-42688+mmc5983ma).jpg" alt="icm-42688+mmc5983ma=9軸センサー" title="icm-42688+mmc5983ma=9軸センサー" />

# 0. はじめに

haya_imu_ros2は、9軸IMU/AHRS haya_imu v3の専用ROS2パッケージです。haya_imuはCortex-M4、ICM-42688、MMC5983MAを搭載しています。主なフィーチャとして、通常出力モード、デモンストレーションモード、キャリブレーションモード、6軸回転ベクトルクォータニオン、9軸回転ベクトルクォータニオン、オイラー角を同時にパブリッシュ(Max1000Hz Best Effort)することと、RVIZにてのデモンストレーションが可能となります。ROSパッケージはリポジトリhaya_imu_rosとして公開しています。

[2024/10/29 v3.4] お客先要望により、パブリッシュメッセージタイプは従来の[haya_imu_msgs::msg::ImuData]の他に、[sensor_msgs::msg::Imu]を新規追加済み。また、params.yamlのパラメータである、message_typeにより切り替えます。

# 1. 対向環境

- Ubuntu 24.04 22.04 / ROS2 jazzy humble 推奨

# 2. 使用手順

## 2.1 haya_imu_ros2のインストール

$cd ~/ros2_ws/src

$git clone https://github.com/soarbear/haya_imu_msgs.git

$git clone https://github.com/soarbear/haya_imu_ros2.git

$cd ~/ros2_ws

$colcon build --packages-select haya_imu_msgs haya_imu_ros2

## 2.2 USBでの接続

haya_imuを対向装置へUSBで接続できた場合、赤LEDが常時点灯することを確認できます。

## 2.3 USBシリアルポート番号の固定

- 以下ルールの作成によって、ttyACM_hayaに固定します。

$chmod +x ~/ros2_ws/src/haya_imu_ros2/script/create_rules.sh

$~/ros2_ws/src/haya_imu_ros2/script/create_rules.sh

$sudo udevadm control --reload-rules && udevadm trigger

- また、以下ルールの削除によって、固定したUSBシリアルポート番号を解除します。

$chmod +x ~/ros2_ws/src/haya_imu_ros2/script/delete_rules.sh

$~/ros2_ws/src/haya_imu_ros2/script/delete_rules.sh

$sudo udevadm control --reload-rules && udevadm trigger

## 2.4 パラメータの確認

params.yamlに載ってあるパラメータの値を確認して、目的、必要に応じて変更してください。

## 2.5 ROS2 LAUNCH

- 通常出力モード、キャリブレーションモードの場合

$source ~/ros2_ws/install/setup.bash

$ros2 launch haya_imu_ros2 haya_imu_launch.py

- デモンストレーションモードの場合

$source ~/ros2_ws/install/setup.bash

$ros2 launch haya_imu_ros2 haya_imu_demo_launch.py

<img src="https://github.com/soarbear/haya_imu_ros2/blob/main/image/demo_fusion.jpg" alt="9軸IMU=icm-42688+mmc5983maデータシュージョンデモ" title="9軸IMU=icm-42688+mmc5983maデータシュージョンデモ" />

## 2.6 Topic、tfの確認

- メッセージのタイプ

imu_data(Message: haya_imu_msgs/msg/ImuData), 通常出力モード、キャリブレーションモード用 

tf(Message: geometry_msgs/msg/TransformStamped), デモンストレーションモード用

- Topicのデータを確認する例

$source ~/ros2_ws/install/setup.bash

$ros2 launch haya_imu_ros2 haya_topic_echo_launch.py

もしくは、$ros2 topic echo /imu_data

- Topicの出力レートを確認する例

$source ~/ros2_ws/install/setup.bash

$ros2 launch haya_imu_ros2 haya_topic_hz_launch.py

もしくは、$ros2 topic hz -w 1000 /imu_data

## 2.7 キャリブレーション

製品マニュアルを参照してください。

## 2.8 (オプション)DDSの切り替え

以下の手順より、Fast DDS(デフォルト)をCyclone DDS(例)あるいは他のDDSへ切り替えます。

- Fast DDS(デフォルト) → Cyclone DDS

$export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp

$printenv RMW_IMPLEMENTATION

$source ~/ros2_ws/install/setup.bash

$ros2 launch haya_imu_ros2 haya_imu_launch.py

- Cyclone DDS → Fast DDS(デフォルト)

$export -n RMW_IMPLEMENTATION

$printenv RMW_IMPLEMENTATION

$source ~/ros2_ws/install/setup.bash

$ros2 launch haya_imu_ros2 haya_imu_launch.py

# 3. リリース
- v3.4 October 2024 mainブランチとしてリリース

- v3.2 April 2023 新規リリース

# 4. ライセンス

- 本ROSパッケージ(haya_imu_ros2)に対して、BSD-3-Clauseが適用されます。

# 5. 詳細情報

<a href="https://store.soarcloud.com/products/detail/137" target="_blank">9軸センサー6軸&9軸回転ベクトル 3軸オイラー角 MAX1000Hz同時出力 ROS/ROS2対応 USB接続 | ROBOT翔</a>
