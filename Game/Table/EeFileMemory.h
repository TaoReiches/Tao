#pragma once

class FileMemory
{
public:
	FileMemory(void);
	~FileMemory(void);

	void	Clear();
	bool	ReSize(int iNewSize);

	char*	GetData() const;
	int		GetSize() const;

	FileMemory& operator =(const FileMemory& rkRight);

protected:
	char*	m_pcData;
	int		m_iSize;
};