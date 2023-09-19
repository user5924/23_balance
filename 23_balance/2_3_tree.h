#include "strings.h"

template <typename X>
class two_three_tree
{
public:
	struct elem
	{
		char* keys[2];
		X data[2];

		elem* past;
		elem* next[3];
		int size;
	};

	two_three_tree()
	{
		start = NULL;
		count = 0;

		str = new char[1];
		str[0] = '\0';
	}

	bool add(char* key, X data)
	{
		int res;
		if (start == NULL)
		{
			//cout << "start\n";
			start = new elem[1];
			create_elem(start, 1, key, data);
		}
		else
		{
			elem* curr = find_elem(key, true);

			if (curr == NULL)
				return false;

			if (curr->size == 1)
			{
				curr->size = 2;
				res = strcmp(key, curr->keys[0]);

				if (res == -1)
				{
					curr->keys[1] = curr->keys[0];
					curr->data[1] = curr->data[0];

					curr->keys[0] = copy_str(key);
					curr->data[0] = data;
				}
				else
				{
					curr->keys[1] = copy_str(key);
					curr->data[1] = data;
				}
			}
			else
			{
				int i;
				elem** buff;
				buff = new elem * [3];

				for (i = 0; i < 3; i += 1)
					buff[i] = new elem[1];

				create_elem(buff[0], 1, key, data);
				create_elem(buff[1], 1, curr->keys[0], curr->data[0]);
				create_elem(buff[2], 1, curr->keys[1], curr->data[1]);

				sort_elem(buff);
				elem* past = curr;

				curr = curr->past;

				while (1)
				{
					delete_elem(past);

					if (curr == NULL)
					{
						start = buff[1];
						buff[1]->next[0] = buff[0];
						buff[0]->past = buff[1];

						buff[1]->next[1] = buff[2];
						buff[2]->past = buff[1];

						free(buff);

						break;
					}
					else if (curr->size == 1)
					{
						curr->size = 2;

						if (curr->next[1] == past)
						{
							curr->next[1] = buff[0];
							buff[0]->past = curr;

							curr->next[2] = buff[2];
							buff[2]->past = curr;

							curr->keys[1] = copy_str(buff[1]->keys[0]);
							curr->data[1] = buff[1]->data[0];
						}
						else
						{
							curr->next[2] = curr->next[1];
							curr->data[1] = curr->data[0];
							curr->keys[1] = curr->keys[0];

							curr->next[0] = buff[0];
							buff[0]->past = curr;

							curr->next[1] = buff[2];
							buff[2]->past = curr;

							curr->keys[0] = copy_str(buff[1]->keys[0]);
							curr->data[0] = buff[1]->data[0];
						}

						delete_elem(buff[1]);
						free(buff);
						break;
					}
					else
					{
						buff = new_buff(buff, curr, past);
						past = curr;
						curr = curr->past;
					}
				}
			}
		}

		count += 1;
		start->past = NULL;
		return true;
	}

	bool remove(char* key)
	{
		elem* curr = find_elem(key, false);

		if (curr == NULL)
		{
			return false;
		}
		else
		{
			int res = strcmp(key, curr->keys[0]);
			int num;
			elem* buff;

			if (res == 0)
				num = 0;
			else
				num = 1;

			int num_2;
			if (curr->next[0] != NULL)
			{
				buff = find_max_left(curr, num);
				if (buff->size == 1)
					num_2 = 0;
				else
					num_2 = 1;

				swap_keys_data(buff, num_2, curr, num);

				curr = buff;
				num = num_2;
			}

			if (curr->size == 2)
			{
				curr->size = 1;
				free(curr->keys[num]);

				if (num == 0)
				{
					curr->keys[0] = curr->keys[1];
					curr->data[0] = curr->data[1];
				}
				count -= 1;
				start->past = NULL;
				return true;
			}
			else
			{
				elem* deleted = curr;
				curr = curr->past;

				delete_elem(deleted);
				int i, i2;
				bool flag;
				elem* tail = NULL;

				while (1)
				{
					if (curr == NULL)
					{
						start = tail;
						count -= 1;
						if(start != NULL)
						     start->past = NULL;
						return true;
					}

					if (curr->size == 1)
					{
						if (curr->next[0] != deleted)
							i = 0;
						else
							i = 1;

						if (curr->next[i]->size == 2)
							flag = true;
						else
						{
							flag = false;
							elem* buff = new elem[1];

							elem** childs = new elem * [3];
							buff->size = 2;

							if (i == 0)
							{
								buff->keys[0] = copy_str(curr->next[0]->keys[0]);
								buff->data[0] = curr->next[0]->data[0];

								buff->keys[1] = copy_str(curr->keys[0]);
								buff->data[1] = curr->data[0];

								childs[0] = curr->next[0]->next[0];
								childs[1] = curr->next[0]->next[1];
								childs[2] = tail;
							}
							else
							{

								buff->keys[1] = copy_str(curr->next[1]->keys[0]);
								buff->data[1] = curr->next[1]->data[0];

								buff->keys[0] = copy_str(curr->keys[0]);
								buff->data[0] = curr->data[0];

								childs[1] = curr->next[1]->next[0];
								childs[2] = curr->next[1]->next[1];
								childs[0] = tail;
							}

							for (i2 = 0; i2 < 3; i2 += 1)
							{
								buff->next[i2] = childs[i2];
								if (childs[i2] != NULL)
									childs[i2]->past = buff;
							}

							tail = buff;

							delete_elem(curr->next[i]);
							deleted = curr;
							curr = curr->past;
							delete_elem(deleted);
							free(childs);
						}
					}
					else
						flag = true;

					if (flag == true)
					{
						build_elems(curr, deleted, tail);
						start->past = NULL;
						break;
					}
				}

			}
		}
		count -= 1;
		return true;
	}
	X get_value(char* key)
	{
		elem* curr = find_elem(key, false);
		if (curr == NULL)
			return NAN;
		else
		{
			if (curr->size == 1 or strcmp(curr->keys[0], key) == 0)
				return curr->data[0];
			else
				return curr->data[1];
		}
	}

	int get_elems_number()
	{
		return count;
	}

	void post(fstream& f)
	{
		if (start == NULL)
		{
			f << "EMPTY BASE!!!\n";
			return;
		}
		int size = 1, fold_size = 1;
		int i = 0, i2, i3;
		int max = log2((X)count) + 2;
		int* lengs = new int[max];
		lengs[0] = 1;
		elem*** buff = new elem * *[max];
		int* elems_num = new int[max];

		for (int i = 0; i < max; i += 1)
			elems_num[i] = 0;
		elems_num[0] = start->size;
		i = 0;
		buff[0] = new elem * [1];
		buff[0][0] = start;

		f << "ELEMENTS IN BASE: " << count << endl << endl;

		while (1)
		{
			i3 = 0;
			if (buff[i][0]->next[0] == NULL)
				break;
			else
			{
				fold_size *= 3;
				buff[i + 1] = new elem * [fold_size];
			}
			int i_nxt = i + 1;

			for (i2 = 0; i2 < size; i2 += 1)
			{
				int max_val = i3 + buff[i][i2]->size + 1;

				int i4 = 0;
				while (i3 < max_val)
				{
					buff[i_nxt][i3] = buff[i][i2]->next[i4];
					elems_num[i_nxt] += buff[i][i2]->next[i4]->size;
					i4 += 1;
					i3 += 1;
				}
			}

			lengs[i_nxt] = i3;
			size = i3;

			i += 1;
		}

		for (i2 = 0; i2 <= i; i2 += 1)
		{
			f << "ALL: " << lengs[i2] << "  blocks, " << elems_num[i2] << " of elems     ";
			for (i3 = 0; i3 < lengs[i2]; i3 += 1)
			{
				f << "    " << (i3 + 1) << ": ";
				see_elem(f, buff[i2][i3]);
				f << " | ";
			}
			f << endl;
		}

		for (i2 = 0; i2 <= i; i2 += 1)
			free(buff[i2]);

		free(buff);
		free(lengs);
		free(elems_num);

		for (i2 = 0; i2 < 5; i2 += 1)
			f << endl;
	}

	void clear()
	{
		if (start != NULL)
		{
			clear_procedure(start);
			start = NULL;
		}
	}

private:

	void clear_procedure(elem* curr)
	{
		if (curr->next[0] != NULL)
		{
			clear_procedure(curr->next[0]);
			clear_procedure(curr->next[1]);

			if(curr->size == 2)
				clear_procedure(curr->next[2]);
		}

		delete_elem(curr);
	}

	void swap_keys_data(elem* ppt_1, int num_1, elem* ppt_2, int num_2)
	{
		swap(ppt_1->keys[num_1], ppt_2->keys[num_2]);
		swap(ppt_1->data[num_1], ppt_2->data[num_2]);
	}

	elem** new_buff(elem** buff, elem* curr, elem* past)
	{
		elem** new_buffer = new elem * [3];
		int i;
		for (i = 0; i < 3; i += 1)
			new_buffer[i] = new elem[1];

		if (curr->next[0] == past)
		{
			create_elem(new_buffer[0], 1, buff[1]->keys[0], buff[1]->data[0]);
			create_elem(new_buffer[1], 1, curr->keys[0], curr->data[0]);
			create_elem(new_buffer[2], 1, curr->keys[1], curr->data[1]);

			new_buffer[0]->next[0] = buff[0];
			buff[0]->past = new_buffer[0];

			new_buffer[0]->next[1] = buff[2];
			buff[2]->past = new_buffer[0];

			new_buffer[2]->next[0] = curr->next[1];
			curr->next[1]->past = new_buffer[2];

			new_buffer[2]->next[1] = curr->next[2];
			curr->next[2]->past = new_buffer[2];
		}
		else if (curr->next[1] == past)
		{
			create_elem(new_buffer[0], 1, curr->keys[0], curr->data[0]);
			create_elem(new_buffer[1], 1, buff[1]->keys[0], buff[1]->data[0]);
			create_elem(new_buffer[2], 1, curr->keys[1], curr->data[1]);

			new_buffer[0]->next[0] = curr->next[0];
			curr->next[0]->past = new_buffer[0];

			new_buffer[0]->next[1] = buff[0];
			buff[0]->past = new_buffer[0];

			new_buffer[2]->next[0] = buff[2];
			buff[2]->past = new_buffer[2];

			new_buffer[2]->next[1] = curr->next[2];
			curr->next[2]->past = new_buffer[2];
		}
		else
		{
			create_elem(new_buffer[0], 1, curr->keys[0], curr->data[0]);
			create_elem(new_buffer[1], 1, curr->keys[1], curr->data[1]);
			create_elem(new_buffer[2], 1, buff[1]->keys[0], buff[1]->data[0]);

			new_buffer[0]->next[0] = curr->next[0];
			curr->next[0]->past = new_buffer[0];

			new_buffer[0]->next[1] = curr->next[1];
			curr->next[1]->past = new_buffer[0];

			new_buffer[2]->next[0] = buff[0];
			buff[0]->past = new_buffer[2];

			new_buffer[2]->next[1] = buff[2];
			buff[2]->past = new_buffer[2];
		}

		delete_elem(buff[1]);
		free(buff);
		return new_buffer;
	}

	elem* find_elem(char* key, bool fl_add)
	{
		elem* curr = start;
		elem* buff = NULL;
		int res, i, num;
		num = 0;

		while (1)
		{
			num += 1;
			for (i = 0; i < curr->size; i += 1)
			{
				res = strcmp(key, curr->keys[i]);
				if (res < 0)
				{
					buff = curr->next[i];
					break;
				}
				else if (res == 0)
				{
					if (fl_add == true)
						return NULL;
					else
						return curr;
				}
			}

			if (i == curr->size)
				buff = curr->next[i];

			if (buff == NULL)
			{
				if (fl_add == true)
					return curr;
				else
					return NULL;
			}
			else
				curr = buff;
		}
	}

	void build_elems(elem* curr, elem* deleted, elem* tail)
	{

		int size;
		int i, i2, i3;
		int t_ppt = 0;
		int p_ppt = 0;
		struct pair
		{
			char* key;
			X data;
		};

		pair* folder = new pair[7];
		elem** tails = new elem * [7];
		elem* buff;

		for (i = 0; i <= curr->size; i += 1)
		{
			buff = curr->next[i];
			if (buff == deleted)
			{
				tails[t_ppt] = tail;
				t_ppt += 1;
			}
			else
			{
				for (i2 = 0; i2 <= buff->size; i2 += 1)
				{
					tails[t_ppt] = buff->next[i2];
					t_ppt += 1;
				}

				for (i2 = 0; i2 < buff->size; i2 += 1)
				{
					folder[p_ppt].key = copy_str(buff->keys[i2]);
					folder[p_ppt].data = buff->data[i2];
					p_ppt += 1;
				}
			}

			if (i != curr->size)
			{
				folder[p_ppt].key = copy_str(curr->keys[i]);
				folder[p_ppt].data = curr->data[i];
				p_ppt += 1;
			}
		}

		if (p_ppt < 5)
			size = 1;
		else
			size = 2;

		elem* center = new elem[1];
		elem** n_elems = new elem * [size + 1];
		for (i = 0; i <= size; i += 1)
			n_elems[i] = new elem[1];

		center->size = size;
		int left = p_ppt - 2 * size - 1;
		for (i = 0; i <= size; i += 1)
		{
			center->next[i] = n_elems[i];
			n_elems[i]->past = center;
			if (left > 0)
			{
				n_elems[i]->size = 2;
				left -= 1;
			}
			else
				n_elems[i]->size = 1;
		}
		p_ppt = 0;
		t_ppt = 0;

		for (i = 0; i <= size; i += 1)
		{
			buff = center->next[i];

			for (i2 = 0; i2 < buff->size; i2 += 1)
			{
				buff->keys[i2] = copy_str(folder[p_ppt].key);
				buff->data[i2] = folder[p_ppt].data;
				p_ppt += 1;
			}

			for (i2 = 0; i2 <= buff->size; i2 += 1)
			{
				buff->next[i2] = tails[t_ppt];
				//cout << "t_ppt = " << t_ppt << endl;
				if (tails[t_ppt] != NULL)
					tails[t_ppt]->past = buff;

				t_ppt += 1;
			}

			if (i != size)
			{
				center->keys[i] = copy_str(folder[p_ppt].key);
				center->data[i] = folder[p_ppt].data;
				p_ppt += 1;
			}
		}

		elem* past = curr->past;
		if (past == NULL)
		{
			center->past = NULL;
			start = center;
		}
		else
		{
			i = 0;
			while (past->next[i] != curr)
				i += 1;

			past->next[i] = center;
			center->past = past;
		}

		for (i = 0; i <= curr->size; i += 1)
		{
			if (curr->next[i] != deleted)
				delete_elem(curr->next[i]);
		}

		delete_elem(curr);
		delete[] n_elems;
		for (i = 0; i < p_ppt; i += 1)
		     delete [] folder[i].key;
		free(folder);
		free(tails);
	}

	elem* find_max_left(elem* curr, int num)
	{
		elem* buff;

		if (num == 0)
			buff = curr->next[0];
		else
			buff = curr->next[1];

		if (buff == NULL)
			return curr;
		else
		{
			while (buff != NULL)
			{
				curr = buff;
				buff = curr->next[curr->size];
			}

			return curr;
		}
	}

	void see_elem(fstream& f, elem* ppt)
	{
		int i;
		f << "size = " << ppt->size << "   ";
		for (i = 0; i < ppt->size; i += 1)
			f << ppt->keys[i] << "   ";
	}

	void  sort_elem(elem** buff)
	{
		int res = strcmp(buff[0]->keys[0], buff[1]->keys[0]);
		if (res > 0)
			swap(buff[0], buff[1]);

		res = strcmp(buff[1]->keys[0], buff[2]->keys[0]);
		if (res > 0)
			swap(buff[1], buff[2]);

		res = strcmp(buff[0]->keys[0], buff[1]->keys[0]);
		if (res > 0)
			swap(buff[0], buff[1]);
	}


	void create_elem(elem* ppt, int size, char* key, X data)
	{
		ppt->size = size;
		ppt->data[0] = data;
		if (key != NULL)
		{
			ppt->keys[0] = new char[length(key) + 1];
			strcpy(ppt->keys[0], key);
		}
		else
		{
			ppt->keys[0] = NULL;
		}

		ppt->past = NULL;
		ppt->next[0] = NULL;
		ppt->next[1] = NULL;
		ppt->next[2] = NULL;
	}

	void delete_elem(elem* ppt)
	{
		for (int i = 0; i < ppt->size; i += 1)
			free(ppt->keys[i]);
		free(ppt);
	}
	elem* start;
	int count;
	char* str;
};

