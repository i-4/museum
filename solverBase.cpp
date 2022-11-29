#include "museum.h"
#include "solverBase.h"


#define NONE    '\0'//�����Ȃ��}�X�̈�
#define CANTPUT 'C' //�Ɩ����u���Ȃ��}�X�̈�
#define LIGHT   'L'//�Ɩ��̈�

void PutRecoder::start(const int mark)
{
	putRecoder.push({ {}, mark });
}
void PutRecoder::recoading(const position& pos, int cellState)
{
	putRecoder.push({ pos, cellState });
}
bool PutRecoder::revert(Museum& museum, const int mark)
{
	for (;;) {
		if (putRecoder.empty()) 
			return false;
		else {
			const auto kariPutedCell = putRecoder.top();
			putRecoder.pop();
			if (kariPutedCell.cellState == mark)
				return true;
			else 
				museum.data[kariPutedCell.pos.y][kariPutedCell.pos.x] = kariPutedCell.cellState;
		}
	}
}
SolverBase::SolverBase(const Museum& museum)
	:museum(museum)
{
}
CellType SolverBase::cellType(const char ch) const
{
	if ('0' <= ch && ch <= '4')	return CellType::NUMBER;
	else if (ch == 'x')			return CellType::BLACK;
	else						return CellType::WHITE;
}
bool SolverBase::limitOfMuseum(const int x, const int y) const
{
	return (0 <= x && x < museum.width) && (0 <= y && y < museum.height);
}
std::vector<position> SolverBase::GetAroundNoneCellsPos(const int x, const int y) const
{
	std::vector<position> vectorNEWS;
	if (limitOfMuseum(x, y - 1) && museum.data[y - 1][x] == NONE)	vectorNEWS.push_back({ x + 0, y - 1 });
	if (limitOfMuseum(x + 1, y) && museum.data[y][x + 1] == NONE)	vectorNEWS.push_back({ x + 1, y + 0 });
	if (limitOfMuseum(x, y + 1) && museum.data[y + 1][x] == NONE)	vectorNEWS.push_back({ x + 0, y + 1 });
	if (limitOfMuseum(x - 1, y) && museum.data[y][x - 1] == NONE)	vectorNEWS.push_back({ x - 1, y + 0 });
	return vectorNEWS;
}
std::vector<position> SolverBase::GetAroundTypeCellsPos(const int x, const int y, CellType celltype) const
{
	std::vector<position> vectorNEWS;
	if (limitOfMuseum(x, y - 1) && cellType(museum.data[y - 1][x]) == celltype)	vectorNEWS.push_back({ x + 0, y - 1 });
	if (limitOfMuseum(x + 1, y) && cellType(museum.data[y][x + 1]) == celltype)	vectorNEWS.push_back({ x + 1, y + 0 });
	if (limitOfMuseum(x, y + 1) && cellType(museum.data[y + 1][x]) == celltype)	vectorNEWS.push_back({ x + 0, y + 1 });
	if (limitOfMuseum(x - 1, y) && cellType(museum.data[y][x - 1]) == celltype)	vectorNEWS.push_back({ x - 1, y + 0 });
	return vectorNEWS;
}
int SolverBase::aroundLightCount(const int x, const int y) const
{
	int count = 0;
	if (limitOfMuseum(x, y - 1) && museum.data[y - 1][x] == LIGHT)	++count;
	if (limitOfMuseum(x + 1, y) && museum.data[y][x + 1] == LIGHT)	++count;
	if (limitOfMuseum(x, y + 1) && museum.data[y + 1][x] == LIGHT)	++count;
	if (limitOfMuseum(x - 1, y) && museum.data[y][x - 1] == LIGHT)	++count;
	return count;
}
bool SolverBase::Illuminated(const int x, const int y) const
{
	if (museum.data[y][x] == LIGHT)
		return true;
	else {
		for (const auto& pos : GetAroundTypeCellsPos(x, y, CellType::WHITE)) {
			const position vec = { pos.x - x, pos.y - y };
			for (int i = 1;; ++i) {
				const int moveX = i * vec.x;
				const int moveY = i * vec.y;
				if (limitOfMuseum(x + moveX, y + moveY)
					&& cellType(museum.data[y + moveY][x + moveX]) == CellType::WHITE) {
					if (museum.data[y + moveY][x + moveX] == LIGHT)
						return true;
				}
				else
					break;
			}
		}
	}
	return false;
}
bool SolverBase::All_Illuminated() const
{
	for (int y = 0; y < museum.height; ++y)
		for (int x = 0; x < museum.width; ++x)
			if (cellType(museum.data[y][x]) == CellType::WHITE) {
				if(Illuminated(x, y))
					continue;
				else 
					return false;
			}
	return true;
}

bool SolverBase::deadWhiteCellexist() const
{
	for (int y = 0; y < museum.height; ++y) {
		for (int x = 0; x < museum.width; ++x) {
			if (museum.data[y][x] == CANTPUT
				&& Illuminated(x, y) == false) {
				//�~�ω\�ȃ}�X�𒲂ׂ�
				if ([&]()->bool
					{
						for (const auto& pos : GetAroundTypeCellsPos(x, y, CellType::WHITE)) {
							const position vec = { pos.x - x, pos.y - y };
							for (int i = 1;; ++i) {
								const int moveX = i * vec.x;
								const int moveY = i * vec.y;
								if (limitOfMuseum(x + moveX, y + moveY)
									&& cellType(museum.data[y + moveY][x + moveX]) == CellType::WHITE) {
									if (museum.data[y + moveY][x + moveX] == NONE)
										return false;
								}
								else
									break;
							}
						}
						return true;
					}())
				{
					return true;
				}
			}
		}
	}
	return false;
}

void SolverBase::setCantPut(const int x, const int y, int recode_sig)
{
	if (recode_sig != putRecoder.NOT_RECODE)
		putRecoder.recoading({ x, y }, museum.data[y][x]);
	museum.data[y][x] = CANTPUT;
	for (const auto& nextPos : GetAroundTypeCellsPos(x, y, CellType::NUMBER)) 
		if ((nextPos.x == x && nextPos.y == y) == false)
			nextNumber.push({ nextPos.x, nextPos.y });
}
void SolverBase::putLight(const int x, const int y, int recode_sig)
{
	if (recode_sig != putRecoder.NOT_RECODE)
		putRecoder.recoading({ x, y }, museum.data[y][x] );
	museum.data[y][x] = LIGHT;
	for (const auto& nextPos : GetAroundTypeCellsPos(x, y, CellType::NUMBER)) 
		if ((nextPos.x == x && nextPos.y == y) == false)
			nextNumber.push({ nextPos.x, nextPos.y });

	for (const auto& cantPutPos : GetAroundTypeCellsPos(x, y, CellType::WHITE)) {
		const position vec = { cantPutPos.x - x, cantPutPos.y - y };
		for (int i = 1;; ++i) {
			const int moveX = i * vec.x;
			const int moveY = i * vec.y;
			if (limitOfMuseum(x + moveX, y + moveY)
			&& cellType(museum.data[y + moveY][x + moveX]) == CellType::WHITE) 
				setCantPut(x + moveX, y + moveY, recode_sig);
			else
				break;
		}
	}
}
bool SolverBase::numberCellsProc(int recode_sig)
{
	for (;;) {
		if (nextNumber.empty())
			return true;
		else {
			const auto numberPos = nextNumber.top();
			nextNumber.pop();
			const int  number        = museum.data[numberPos.y][numberPos.x] - '0';
			const auto lightCount    = aroundLightCount(numberPos.x, numberPos.y);
			const auto noneCellsNEWS = GetAroundNoneCellsPos(numberPos.x, numberPos.y);
			const auto noneCellCount = static_cast<int>(noneCellsNEWS.size());
			//�����}�X�̏㉺���E�̏Ɩ��̐��������ƈ�v���Ă���Ƃ��A���͂�None�}�X�ɂ͏Ɩ���u���Ȃ�
			if (lightCount == number) {
				for (const auto& pos : noneCellsNEWS)
					setCantPut(pos.x, pos.y, recode_sig);
			}//����̏Ɩ��̐� + None�}�X�̐��Ɛ�������v���Ă���Ƃ��ANone�}�X�ɏƖ���u��
			else if (lightCount + noneCellCount == number) {
				for (const auto& pos : noneCellsNEWS)
					putLight(pos.x, pos.y, recode_sig);
			}//����ɏƖ����Ȃ��A�����}�X�̏㉺���E��None�}�X�̐�������-1�̂Ƃ��ANone�}�X�ɋ��܂ꂽ�΂߂ɂ͏Ɩ���u���Ȃ�
			else if (number == noneCellCount - 1 && lightCount == 0) {
				for (int i = 0; i < noneCellCount; ++i) {
					const position vec1     = { noneCellsNEWS[i].x - numberPos.x, noneCellsNEWS[i].y - numberPos.y };
					const position vec2     = { noneCellsNEWS[(i + 1) % noneCellCount].x - numberPos.x, noneCellsNEWS[(i + 1) % noneCellCount].y - numberPos.y };
					const position diagonal = { numberPos.x + vec1.x + vec2.x                         , numberPos.y + vec1.y + vec2.y };
					if (vec1.x * vec2.x + vec1.y * vec2.y == 0
					&& limitOfMuseum(diagonal.x, diagonal.y)
					&& cellType(museum.data[diagonal.y][diagonal.x]) == CellType::WHITE
					   )
					   setCantPut(diagonal.x, diagonal.y, recode_sig);
				}
			}//���͂�None�}�X�ƏƖ��̍��v�������}�X��菬�����Ƃ��A���u�����Ԉ���Ă���̂ŁAnextNumber���N���A����false��Ԃ�
			else if (noneCellCount + lightCount < number) {
				while (!nextNumber.empty())
					nextNumber.pop();
				return false;
			}
		}
	}
}
bool SolverBase::logicProc()
{
	//�����}�X�Ɋ֌W����A�K���Ɩ����u�����ꏊ�A�K���u����Ȃ��ꏊ�����߂�
	for (int y = 0; y < museum.height; ++y)
		for (int x = 0; x < museum.width; ++x)
			if (cellType(museum.data[y][x]) == CellType::NUMBER) 
				nextNumber.push({ x, y });
	
	numberCellsProc(putRecoder.NOT_RECODE);

	//�c�����}�X�̋~�ω\�ȃ}�X�𒲂ׂ�
	whiteCellsProc(putRecoder.NOT_RECODE);

	return All_Illuminated();
}
bool SolverBase::whiteCellsProc(int recode_sig)
{
	if (recode_sig != putRecoder.NOT_RECODE)
		putRecoder.start(recode_sig);
	for (;;) {
		const auto whiteCells = [&]()->std::vector<std::tuple<position, int>>
		{
			std::vector<std::tuple<position, int>> whiteCells;
			for (int y = 0; y < museum.height; ++y) {
				for (int x = 0; x < museum.width; ++x) {
					if (cellType(museum.data[y][x]) == CellType::WHITE
					 && Illuminated(x, y) == false) {
						//�~�ω\�ȃ}�X�𒲂ׂ�
						std::vector<position> reliefCellPos;
						for (const auto& pos : GetAroundTypeCellsPos(x, y, CellType::WHITE)) {
							const position vec = { pos.x - x, pos.y - y };
							for (int i = 1;; ++i) {
								const int moveX = i * vec.x;
								const int moveY = i * vec.y;
								if (limitOfMuseum(x + moveX, y + moveY)
								&&  cellType(museum.data[y + moveY][x + moveX]) == CellType::WHITE) {
									if (museum.data[y + moveY][x + moveX] == NONE)
										reliefCellPos.push_back({ x + moveX, y + moveY });
								}
								else
									break;
							}
						}
						const int size = static_cast<int>(reliefCellPos.size());
						if (size == 0)
							whiteCells.push_back(std::tuple<position, int>{ { x, y }, size });
						else if(reliefCellPos.size() == 1 && museum.data[y][x] == CANTPUT) 
							whiteCells.push_back(std::tuple<position, int>{ reliefCellPos[0], size });
					}
				}
			}
			return whiteCells;
		}();
		if (whiteCells.size()) {
			for (const auto whiteCell : whiteCells) {
				const auto pos = std::get<0>(whiteCell);
				const auto num = std::get<1>(whiteCell);
				if (num == 0) {
					//�����_�B���̃}�X��CANTPUT�ł��Ƃ炳���\���̂Ȃ����蓾�Ȃ��}�X�ł���̂ŁAreturn falsel
					if (museum.data[pos.y][pos.x] == CANTPUT) 
						return false;
					else {//�Ǘ��_�B�~�ω\�}�X�����݂��Ȃ��Ƃ��A�����ɏƖ���u��
						putLight(pos.x, pos.y, recode_sig);
						numberCellsProc(recode_sig);
					}
				}//���̃}�X�ɂ͏Ɩ���u���Ȃ����~�ω\�}�X����̂Ƃ��́A�~�ω\�}�X�ɏƖ���u��
				else  {
					putLight(pos.x, pos.y, recode_sig);
					numberCellsProc(recode_sig);
				}
			}
		}
		else
			return true;
	}
}

bool SolverBase::kariPut(std::vector<position> combKariPutPos, int recode_sig)
{
	putRecoder.start(recode_sig);
	const int size = static_cast<int>(combKariPutPos.size());
	for (int i = 0; i < size; ++i) {
		putLight(combKariPutPos[i].x, combKariPutPos[i].y, true);
		if (numberCellsProc(true)) {
			if (deadWhiteCellexist())
				return false;
			else
				continue;
		}
		else
			return false;
	}
	return true;
}
std::vector<std::vector<position>> SolverBase::combination(const int number, const std::vector<position>& kariPutPos)
{
	//noneVector����number���g�ݍ��킹�����߂�
	std::vector<int> permutation(kariPutPos.size());
	for (int i = 0; i < number; ++i)
		permutation[i] = 1;
	std::sort(permutation.begin(), permutation.end());
	std::vector<std::vector<position>> retVector;
	do {
		std::vector<position> combkariPutPos;
		for (int i = 0; i < kariPutPos.size(); ++i)
			if (permutation[i] == 1)
				combkariPutPos.push_back(kariPutPos[i]);
		retVector.push_back(combkariPutPos);
	} while (std::next_permutation(permutation.begin(), permutation.end()));
	
	return retVector;
}
position SolverBase::nextKariPutNumbersPos(const int _x, const int _y)
{
	const int museumSize = museum.width * museum.height - 1;
	for (int index = _y * museum.width + _x; index < museumSize; ++index) {
		const int nextX = index % museum.width;
		const int nextY = index / museum.width;
		if (cellType(museum.data[nextY][nextX]) == CellType::NUMBER) {
			if (aroundLightCount(nextX, nextY) == museum.data[nextY][nextX] - '0')
				continue;
			else
				return { nextX, nextY };
		}
	}
	return {-1, -1};
}
bool SolverBase::kariProc(const position& pos)
{
	const auto number = museum.data[pos.y][pos.x] - '0';
	for (const auto& kariPutPosV : combination(number - aroundLightCount(pos.x, pos.y), GetAroundNoneCellsPos(pos.x, pos.y))) {
		if (kariPut(kariPutPosV, putRecoder.START_SIG)) {
			const auto nextKariPutPos = nextKariPutNumbersPos(pos.x, pos.y);
			if (nextKariPutPos.x == -1 && nextKariPutPos.y == -1) {
				if (whiteCellsProc(putRecoder.MARK_SIG)) {
					//None�}�X�ɂ��āA���u�����Ă���
					if (All_Illuminated()) {
						//����
						//�Ɩ��̐��𐔂��āA���݂̏Ɩ��̍ŏ�����菬�����Ȃ�΁Amuseum�f�[�^�ƏƖ��̐���ۑ�����.
						return true;
					}
					else {
						putRecoder.revert(museum, putRecoder.MARK_SIG);
						putRecoder.revert(museum, putRecoder.START_SIG);
					}
				}
				else {
					putRecoder.revert(museum, putRecoder.MARK_SIG);
					putRecoder.revert(museum, putRecoder.START_SIG);
				}
			}
			else {
				if (kariProc(nextKariPutPos))
					return true;
				else
					putRecoder.revert(museum, putRecoder.START_SIG);
			}
		}
		else 
			putRecoder.revert(museum, putRecoder.START_SIG);
	}
	return false;
}
Museum SolverBase::solve()
{
	//------------------------------���W�b�N���[�h-------------------------------------//
	if (logicProc() == false) {
		//------------------------------���u�����[�h-------------------------------------//
		if (kariProc(nextKariPutNumbersPos(0, 0)) == false)
			throw std::runtime_error("��������܂���ł����B�t�@�C���`�����m�F���Ă�������");		
	}
	///�����ŉ����̂͏I��//
	for (int y = 0; y < museum.height; ++y)
		for (int x = 0; x < museum.width; ++x)
			if (cellType(museum.data[y][x]) == CellType::WHITE) {
				if (museum.data[y][x] == LIGHT)
					museum.data[y][x] = 'o';//�Ɩ��uo�v��z�u
				else
					museum.data[y][x] = '\0';
			}
	return museum;
}

#undef CANTPUT
#undef LIGHT
#undef NONE