# Setup

このプロジェクトをこの環境（Ubuntu系）でセットアップする手順です。

## 1. 依存パッケージ導入

```bash
sudo apt update
sudo apt install -y build-essential clang cmake ninja-build git pkg-config libvulkan-dev
```

## 2. ビルドと実行

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/vulkan_sample
```

## 3. セットアップスクリプト

この環境で使ったセットアップコマンドは次のスクリプトにまとめています。

```bash
bash scripts/setup_env.sh
```

