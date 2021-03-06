#include "vmt.h"

namespace VMT
{
	VMTHookManager::VMTHookManager(void* instance)
	{
		Init(instance);
	}
	VMTHookManager::~VMTHookManager()
	{
		Restore();
	}

	void VMTHookManager::Init(void* instance)
	{
		m_instance = instance;
		m_vtable = *static_cast<void***>(instance);

		/// gets the ammount of vfuncs in the vtable
		auto GetSize = [this]() -> int
		{
			int count = 0;
			for (void* curr_func = m_vtable[count]; curr_func != nullptr;)
			{
				MEMORY_BASIC_INFORMATION mbi;
				if (!VirtualQuery(curr_func, &mbi, sizeof(mbi)))
					break;

				/// offlimits
				if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
					break;

				/// not readable
				if (!(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_READONLY | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_WRITECOPY)))
					break;

				count++;
				curr_func = m_vtable[count];
			}

			return count;
		}; m_size = GetSize();
	}
	void VMTHookManager::Restore()
	{
		if (!m_instance || !m_vtable)
			return;

		/// restore all the original functions to the vtable
		for (const auto &func : m_original_funcs)
		{
			unsigned long old_protection, tmp;
			VProtect(&m_vtable[func.first], sizeof(void*), PAGE_EXECUTE_READWRITE, &old_protection);
			m_vtable[func.first] = func.second;
			VProtect(&m_vtable[func.first], sizeof(void*), old_protection, &tmp);
		}

		m_original_funcs.clear();
	}
	void VMTHookManager::UnHookFunction(int index)
	{
		/// index is out of range
		if (index < 0 || index >= m_size)
			return;

		/// not even hooked retard
		if (m_original_funcs.find(index) == m_original_funcs.end())
			return;

		auto original = m_original_funcs[index];
		m_original_funcs.erase(index);

		unsigned long old_protection, tmp;
		VProtect(&m_vtable[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &old_protection);
		m_vtable[index] = original;
		VProtect(&m_vtable[index], sizeof(void*), old_protection, &tmp);
	}
	void VMTHookManager::VProtect(void* address, unsigned int size, unsigned long new_protect, unsigned long* old_protect)
	{
		VirtualProtect(address, size, new_protect, old_protect);
	}
	/*CVMTHookManager::CVMTHookManager() {}
	CVMTHookManager::~CVMTHookManager()
	{
		this->HookTable(false);
	}
	CVMTHookManager::CVMTHookManager(void* Interface)
	{
		this->Init(Interface);
	}
	bool CVMTHookManager::Init(void* Interface)
	{
		pOrgTable = *(void**)Interface;
		this->count = this->GetCount();
		pCopyTable = malloc(sizeof(void*)* count);
		memcpy(pCopyTable, pOrgTable, sizeof(void*) * count);
		pObject = (DWORD*)Interface;
		return true;
		/*m_ClassBase = Interface;
		m_OldVT = *Interface;
		m_VTSize = GetVTCount(*Interface);
		m_NewVT = new DWORD[m_VTSize + 1];
		memcpy(&m_NewVT[1], m_OldVT, sizeof(DWORD) * m_VTSize);
		m_NewVT[0] = (uintptr_t)m_OldVT[-1];
		DWORD old;
		VirtualProtect(Interface, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		*Interface = &m_NewVT[1];
		VirtualProtect(Interface, sizeof(DWORD), old, &old);
		return true;*/
	/*}
	void CVMTHookManager::HookTable(bool hooked)
	{
		if (hooked)
			*pObject = (DWORD)pCopyTable;
		else
			*pObject = (DWORD)pOrgTable;
	}
	void* CVMTHookManager::HookFunction(int Index, void* hkFunction)
	{
		if (Index < this->count && Index >= 0)
		{
			((DWORD*)pCopyTable)[Index] = (DWORD)hkFunction;
			return (void*)((DWORD*)pOrgTable)[Index];
		}
		return NULL;
	}
	bool CVMTHookManager::CanReadPointer(void* table)
	{
		MEMORY_BASIC_INFORMATION mbi;

		if (table == nullptr)
			return false;
		if (!VirtualQuery(table, &mbi, sizeof(mbi))) 
			return false;
		if (mbi.Protect & k_page_offlimits) 
			return false;

		return mbi.Protect & k_page_readable;
	}
	int CVMTHookManager::GetCount()
	{
		int index = 0; //0
		void** table = ((void**)pOrgTable);
		for (void* fn; (fn = table[index]) != nullptr; index++)
		{
			if (!this->CanReadPointer(fn)) 
				break;
		}
		return index;
	}
	DWORD CVMTHookManager::GetVTCount(PDWORD pdwVMT)
	{
		DWORD dwIndex = 0;
		for (dwIndex = 0; pdwVMT[dwIndex]; dwIndex++)
		{
			if (IS_INTRESOURCE((FARPROC)pdwVMT[dwIndex]))
				break;
			}
		return dwIndex;
	}*/
}



auto table_is_hooked(void* table, const char* module_table_dst) -> const bool
{
	// Because of this probable check you can make a VEH Hook by forcing the pointer to 0
	if (table == nullptr) // Not initialized or invalid table to check
		return false;

	const auto module_base = GetModuleHandleA(module_table_dst);

	// Get module end
	const auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER > (module_base);
	const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS > (reinterpret_cast< std::uint8_t* >(module_base) + dos_header->e_lfanew);

	const auto module_end = reinterpret_cast< std::uintptr_t >(module_base) + nt_headers->OptionalHeader.SizeOfImage - sizeof(std::uintptr_t);

	const auto table_dst = *reinterpret_cast< std::uintptr_t* >(table);

	// Check the destination of the table, if the destination of the table is outside the region of the module, it is because it has been changed(hooked)
	return (table_dst < reinterpret_cast< std::uintptr_t >(module_base) || table_dst > module_end);
}
uintptr_t* vfunc_hook::search_free_data_page(const char* module_name, const std::size_t vmt_size) //Modified code from exphck https://www.unknowncheats.me/forum/2128832-post43.html
{
	auto check_data_section = [&](LPCVOID address, const std::size_t vmt_size)
	{
		const DWORD DataProtection = (PAGE_EXECUTE_READWRITE | PAGE_READWRITE);
		MEMORY_BASIC_INFORMATION mbi = { 0 };

		if (VirtualQuery(address, &mbi, sizeof(mbi)) == sizeof(mbi) && mbi.AllocationBase && mbi.BaseAddress &&
			mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_GUARD) && mbi.Protect != PAGE_NOACCESS)
		{
			if ((mbi.Protect & DataProtection) && mbi.RegionSize >= vmt_size)
			{
				return ((mbi.Protect & DataProtection) && mbi.RegionSize >= vmt_size) ? true : false;
			}
		}
		return false;
	};

	auto module_addr = GetModuleHandleA(module_name);

	if (module_addr == nullptr)
		return nullptr;

	const auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER > (module_addr);
	const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS > (reinterpret_cast< std::uint8_t* >(module_addr) + dos_header->e_lfanew);

	const auto module_end = reinterpret_cast< std::uintptr_t >(module_addr) + nt_headers->OptionalHeader.SizeOfImage - sizeof(std::uintptr_t);

	for (auto current_address = module_end; current_address > (DWORD)module_addr; current_address -= sizeof(std::uintptr_t))
	{
		if (*reinterpret_cast< std::uintptr_t* >(current_address) == 0 && check_data_section(reinterpret_cast< LPCVOID >(current_address), vmt_size))
		{
			bool is_good_vmt = true;
			auto page_count = 0u;

			for (; page_count < vmt_size && is_good_vmt; page_count += sizeof(std::uintptr_t))
			{
				if (*reinterpret_cast< std::uintptr_t* >(current_address + page_count) != 0)
					is_good_vmt = false;
			}

			if (is_good_vmt && page_count >= vmt_size)
				return (uintptr_t*)current_address;
		}
	}
	return nullptr;
}

vfunc_hook::vfunc_hook()
	: class_base(nullptr), vftbl_len(0), new_vftb1(nullptr), old_vftbl(nullptr)
{
}
vfunc_hook::vfunc_hook(void* base)
	: class_base(base), vftbl_len(0), new_vftb1(nullptr), old_vftbl(nullptr)
{
}
vfunc_hook::~vfunc_hook()
{
	unhook_all();
	if (wasAllocated)
		delete[] new_vftb1;
}

bool vfunc_hook::setup(void* base, const char * moduleName)
{
	if (base != nullptr)
		class_base = base;

	if (class_base == nullptr)
		return false;

	old_vftbl = *(std::uintptr_t**)class_base;
	vftbl_len = estimate_vftbl_length(old_vftbl) * sizeof(std::uintptr_t);

	if (vftbl_len == 0)
		return false;


	if (moduleName)// If provided a module name then we will find a place in that module				
	{
		new_vftb1 = search_free_data_page(moduleName, vftbl_len + sizeof(std::uintptr_t));
		std::memset(new_vftb1, NULL, vftbl_len + sizeof(std::uintptr_t));
		std::memcpy(&new_vftb1[1], old_vftbl, vftbl_len);
		new_vftb1[0] = old_vftbl[-1];
		try {
			auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };

			*(std::uintptr_t**)class_base = &new_vftb1[1];
			wasAllocated = false;
			if (table_is_hooked(base, moduleName))
			{
				Beep(500, 500);
			}
		}
		catch (...) {
			delete[] new_vftb1;
			return false;
		}
	}
	else // If not then we do regular vmthookinh
	{
		new_vftb1 = new std::uintptr_t[vftbl_len + 1]();
		std::memcpy(&new_vftb1[1], old_vftbl, vftbl_len);
		try {
			auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
			new_vftb1[0] = old_vftbl[-1];
			*(std::uintptr_t**)class_base = &new_vftb1[1];
			wasAllocated = true;
		}
		catch (...) {
			delete[] new_vftb1;
			return false;
		}
	}


	return true;
}

std::size_t vfunc_hook::estimate_vftbl_length(std::uintptr_t* vftbl_start)
{
	MEMORY_BASIC_INFORMATION memInfo = { NULL };
	int m_nSize = -1;
	do {
		m_nSize++;
		VirtualQuery(reinterpret_cast<LPCVOID>(vftbl_start[m_nSize]), &memInfo, sizeof(memInfo));

	} while (memInfo.Protect == PAGE_EXECUTE_READ || memInfo.Protect == PAGE_EXECUTE_READWRITE);

	return m_nSize;
}


