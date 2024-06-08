#pragma once
#include "DirectXCommon.h"
#include <assert.h>
#include "Vector3.h"

enum class MapChipType {
	kBlank,//空白
	kBlock,//ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct Rect {
	float left; //左端
	float right; //右端
	float bottom; //下端
	float top; //上端
};

class MapChipField {
public:
	/// <summary>
	/// 読み込み済みのデータ削除
	/// </summary>
	void ResetMapChipData();

	/// <summary>
	/// ファイル読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	///	マップチップ種別を取得
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// マップチップのワールド座標を取得する関数
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// 指定ブロックの全方向の教会の座標を求める
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex); 
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; };
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; };


private:
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

};
