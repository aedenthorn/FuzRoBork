// SME::Functors - Basic functor implementations

#ifndef __SME_SUNDRIES_FUNCTORS_H__
#define __SME_SUNDRIES_FUNCTORS_H__
namespace SME
{
	namespace Functors
	{
		class GenericFunctorBase
		{
		public:
			virtual ~GenericFunctorBase()
			{
				;//
			}
		};

		class VoidRFunctorBase : public GenericFunctorBase
		{
		public:
			virtual ~VoidRFunctorBase()
			{
				;//
			}

			virtual void		operator()() = 0;
		};

		class BoolRFunctorBase : public GenericFunctorBase
		{
		public:
			virtual ~BoolRFunctorBase()
			{
				;//
			}

			virtual bool		operator()() = 0;
		};

		class UInt32RFunctorBase : public GenericFunctorBase
		{
		public:
			virtual ~UInt32RFunctorBase()
			{
				;//
			}

			virtual UInt32		operator()() = 0;
		};

		template <typename T>
		class GetterTFunctorBase : public GenericFunctorBase
		{
		protected:
			T					Value;
		public:
			GetterTFunctorBase(T Value) : Value(Value) {}
			virtual ~GetterTFunctorBase()
			{
				;//
			}

			virtual T			operator()()
			{
				return Value;
			}
		};

		// some basic instantiations
		typedef GetterTFunctorBase<void*>						PointerGetter;
		typedef GetterTFunctorBase<HWND>						HWNDGetter;
		typedef GetterTFunctorBase<HINSTANCE>					HINSTANCEGetter;
		typedef GetterTFunctorBase<HMENU>						HMENUGetter;
		typedef GetterTFunctorBase<LPDIRECT3DDEVICE9>			D3D9DeviceGetter;
	}
}
#endif