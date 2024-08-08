int main ()
{
	
	return 0;
}

static long _Abs (long x);
static vsa_t *_JumpNext(vsa_t *index);
static long _FragmentationFix(vsa_t *runner, vsa_t *next_index);
static size_t _AlignBlock(size_t size_of_block);
static int _BlockSizeChecks(vsa_t *runner, vsa_t *next_index);

static size_t _AlignBlock(size_t size_of_block)
{
	size_of_block = (size_of_block + ALIGNMENT - 1) & ~(ALIGNMENT - 1 );
	return size_of_block;
}

static long _Abs (long x)
{
	return x >= 0 ? x : -x;
}

static vsa_t *_JumpNext(vsa_t *index)
{
	return (vsa_t *)((char *)index + _Abs(index->mem_count) + HEADER_SIZE);
}

static long _FragmentationFix(vsa_t *runner, vsa_t *next_index)
{
	return runner->mem_count += next_index->mem_count + HEADER_SIZE;	
}

static int _BlockSizeChecks(vsa_t *runner, vsa_t *next_index)
{
	if(runner->mem_count < 0)
	{
		runner = _JumpNext(runner);
		next_index = _JumpNext(runner);
		return 1;
	}

	if (next_index->mem_count > 0)
	{
		runner->mem_count =_FragmentationFix(runner, next_index);
		next_index = _JumpNext(runner);
		return 1;
	}
	return 0;
}
