#include "VolumeInfo.h"

uint32_t const VolumeInfo::DefaultSignature = 0x02444448;//0x0254484E;//HDD = 0x02444448

VolumeInfo::VolumeInfo()
{
	this->Signature			= this->DefaultSignature;
	this->SizeEntryTable	= 0;
	this->OffsetEntryTable	= 0;
}

VolumeInfo::~VolumeInfo() {}

bool VolumeInfo::isEmptyVolume() const
{
	return this->SizeEntryTable == 0;
}

uint64_t VolumeInfo::getEntryTableOffset() const
{
	return this->OffsetEntryTable;
}

void VolumeInfo::write(fstream& file) const
{
	file.write((char*)&this->Signature, sizeof(this->Signature));
	file.write((char*)&this->SizeEntryTable, sizeof(this->SizeEntryTable));
	file.write((char*)&this->OffsetEntryTable, sizeof(this->OffsetEntryTable));
	//file.write((char*)this->volPass.c_str(), sizeof(this->volPass));
}

void VolumeInfo::read(fstream& file) const
{
	file.read((char*)&this->Signature, sizeof(this->Signature));
	file.read((char*)&this->SizeEntryTable, sizeof(this->SizeEntryTable));
	file.read((char*)&this->OffsetEntryTable, sizeof(this->OffsetEntryTable));

	//file.read((char*)this->volPass.c_str(), sizeof(this->volPass));
}

bool VolumeInfo::checkSignature(fstream& file) const
{
	return this->Signature == this->DefaultSignature;
}

void VolumeInfo::seekToHeadOfEntryTable_g(fstream& file) const
{
	file.seekg(this->OffsetEntryTable);
}

void VolumeInfo::seekToHeadOfEntryTable_p(fstream& file) const
{
	file.seekp(this->OffsetEntryTable);
}

bool VolumeInfo::isEndOfEntryTable_g(fstream& file) const
{
	return file.tellg() == (this->OffsetEntryTable + this->SizeEntryTable);
}

void VolumeInfo::updateAfterDel(Entry const* entry)
{
	this->SizeEntryTable -= entry->getSize();
	this->OffsetEntryTable -= entry->getSizeData();
}

void VolumeInfo::updateAfterSetPassword(size_t const& oldPasswordLen, size_t const& newPasswordLen)
{
	this->SizeEntryTable += (newPasswordLen - oldPasswordLen);
}

void VolumeInfo::updateOffsetEntryTable(uint64_t const& newOffsetEntryTable)
{
	this->OffsetEntryTable = newOffsetEntryTable;
}

void VolumeInfo::updateSizeEntryTable(uint64_t const& newTailPosEntryTable)
{
	this->SizeEntryTable = newTailPosEntryTable - this->OffsetEntryTable;
}
