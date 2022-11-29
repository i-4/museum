#pragma once
#include <stack>
#include <algorithm>
#include <tuple>

enum class CellType {
	NUMBER,
	BLACK,
	WHITE
};
struct position {
	int x;
	int y;
};

class PutRecoder {
	struct RecoadData {
		position pos;
		int cellState;
	};
	std::stack<RecoadData> putRecoder;
public:
	const int NOT_RECODE = 'N';
	const int START_SIG  = 'S';
	const int MARK_SIG   = 'M';
	void start    (const int       mark                       );
	void recoading(const position& pos   , int       cellState);
	bool revert   (Museum&         museum, const int mark     );
};
class SolverBase {
public:
	Museum solve();
	SolverBase  (const Museum& museum);
private:
	Museum               museum;
	std::stack<position> nextNumber;
	PutRecoder           putRecoder;
	

	CellType              cellType             (const char ch                              ) const;
	bool                  limitOfMuseum        (const int x, const int y                   ) const;
	std::vector<position> GetAroundNoneCellsPos(const int x, const int y                   ) const;
	std::vector<position> GetAroundTypeCellsPos(const int x, const int y, CellType celltype) const;
	int                   aroundLightCount     (const int x, const int y                   ) const;
	bool                  Illuminated          (const int x, const int y                   ) const;
	bool                  All_Illuminated      (                                           ) const;
	bool                  deadWhiteCellexist   (                                           ) const;

	void setCantPut     (const int x, const int y,  int recode_sig);
	void putLight       (const int x, const int y,  int recode_sig);
	bool numberCellsProc(                           int recode_sig);
	bool whiteCellsProc (                           int recode_sig);
	bool kariPut        (std::vector<position> pos, int recode_sig);

	position                           nextKariPutNumbersPos(const int x     , const int                    y         );
	std::vector<std::vector<position>> combination          (const int number, const std::vector<position>& kariPutPos);
	bool                               logicProc            (                                                         );
	bool                               kariProc             (const position& pos                                      );
};
