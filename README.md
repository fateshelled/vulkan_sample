# vulkan_sample

Vulkan-Hpp + volk + VMA（Vulkan Memory Allocator）を組み合わせた最小サンプルです。

## サンプルの概要

このサンプルは次の最小フローを実装しています。

1. `volkInitialize()` で Vulkan ローダーを初期化
2. Vulkan-Hpp で `Instance` を作成
3. `volkLoadInstance` と Vulkan-Hpp dispatcher の初期化
4. 物理デバイス選択と `Device` 作成
5. `volkLoadDevice` と Vulkan-Hpp dispatcher の初期化
6. VMA (`VmaAllocator`) を作成
7. 1KB の `VkBuffer` を VMA で確保・解放

実装ファイル:

- `src/main.cpp`: 初期化からバッファ確保/解放まで
- `src/vma_impl.cpp`: `VMA_IMPLEMENTATION` の単一定義

## ビルド

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

## 実行

```bash
./build/vulkan_sample
```

成功時の出力:

```text
Sample finished successfully.
```

## セットアップ手順

セットアップ手順は `Setup.md` を参照してください。

- 依存パッケージ導入
- この環境で使用したコマンドをまとめたスクリプト
