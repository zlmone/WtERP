#ifndef GS_POSITION_MVC_H
#define GS_POSITION_MVC_H

#include "Dbo/Dbos.h"
#include "Utilities/QueryProxyMVC.h"
#include "Utilities/RecordFormView.h"
#include "Utilities/FilteredList.h"

#include <Wt/WTemplateFormView>
#include <Wt/WBatchEditProxyModel>
#include <Wt/WDateValidator>

namespace GS
{
	class EntryCycleView;
	class ExpenseCycleView;
	class IncomeCycleView;
	class EmployeeAssignmentFormModel;
	class PositionView;
	class ServiceView;

	//PositionProxyModel
	class PositionProxyModel : public QueryProxyModel<Wt::Dbo::ptr<EmployeePosition>>
	{
	public:
		PositionProxyModel(Wt::Dbo::QueryModel<Wt::Dbo::ptr<EmployeePosition>> *sourceModel, Wt::WObject *parent = nullptr);

	protected:
		void addAdditionalRows();
	};

	//PositionView
	class PositionFormModel : public RecordFormModel<EmployeePosition>
	{
	public:
		static const Wt::WFormModel::Field titleField;

		PositionFormModel(PositionView *view, Wt::Dbo::ptr<EmployeePosition> positionPtr = Wt::Dbo::ptr<EmployeePosition>());
		virtual Wt::WWidget *createFormWidget(Field field) override;
		virtual bool saveChanges() override;

	protected:
		PositionView *_view = nullptr;
	};

	class PositionView : public RecordFormView
	{
	public:
		PositionView(Wt::Dbo::ptr<EmployeePosition> positionPtr);
		PositionView();
		virtual void init() override;

		Wt::Dbo::ptr<EmployeePosition> positionPtr() const { return _model->recordPtr(); }
		Wt::WFormModel *model() const { return _model; }

	protected:
		PositionFormModel *_model = nullptr;
		Wt::Dbo::ptr<EmployeePosition> _tempPtr;
	};

	//ServiceProxyModel
	class ServiceProxyModel : public QueryProxyModel<Wt::Dbo::ptr<ClientService>>
	{
	public:
		ServiceProxyModel(Wt::Dbo::QueryModel<Wt::Dbo::ptr<ClientService>> *sourceModel, Wt::WObject *parent = nullptr);

	protected:
		void addAdditionalRows();
	};

	//ServiceView
	class ServiceFormModel : public RecordFormModel<ClientService>
	{
	public:
		static const Wt::WFormModel::Field titleField;

		ServiceFormModel(ServiceView *view, Wt::Dbo::ptr<ClientService> servicePtr = Wt::Dbo::ptr<ClientService>());
		virtual Wt::WWidget *createFormWidget(Field field) override;
		virtual bool saveChanges() override;

	protected:
		ServiceView *_view = nullptr;
	};
	class ServiceView : public RecordFormView
	{
	public:
		ServiceView(Wt::Dbo::ptr<ClientService> servicePtr);
		ServiceView();
		virtual void init() override;

		Wt::Dbo::ptr<ClientService> servicePtr() const { return _model->recordPtr(); }
		ServiceFormModel *model() const { return _model; }

	protected:
		ServiceFormModel *_model = nullptr;
		Wt::Dbo::ptr<ClientService> _tempPtr;
	};

	class CycleEndDateValidator : public Wt::WDateValidator
	{
	public:
		CycleEndDateValidator(Wt::WFormModel *model) : Wt::WDateValidator(model), _model(model) { }
		virtual Result validate(const Wt::WString &input) const override;

	protected:
		Wt::WFormModel *_model = nullptr;
	};

	//EntryCycleFormModel
	class EntryCycleFormModel
	{
	public:
		static const Wt::WFormModel::Field entityField;
		static const Wt::WFormModel::Field startDateField;
		static const Wt::WFormModel::Field endDateField;
		static const Wt::WFormModel::Field intervalField;
		static const Wt::WFormModel::Field nIntervalsField;
		static const Wt::WFormModel::Field amountField;
		static const Wt::WFormModel::Field firstEntryAfterCycleField;

		static void addFields(Wt::WFormModel *model);
		static Wt::WWidget *createFormWidget(Wt::WFormModel *model, EntryCycleView *view, Wt::WFormModel::Field field);
		static void updateEndDateValidator(Wt::WFormModel *model, EntryCycleView *view, bool update);
		static void updateModelFromCycle(Wt::WFormModel *model, const EntryCycle &cycle);

		template<class Dbo>
		static void updateCycleFromModel(Wt::WFormModel *model, Wt::Dbo::ptr<Dbo> ptr, bool newCycle);
	};

	//EntryCycleView
	class EntryCycleView : public RecordFormView
	{
	public:
		using RecordFormView::updateView;
		using RecordFormView::updateModel;

	protected:
		EntryCycleView(const Wt::WString &text) : RecordFormView(text) { }
		virtual void updateView(Wt::WFormModel *model) override;
		virtual void updateModel(Wt::WFormModel *model) override;
		void handleIntervalChanged();

	private:
		friend class EntryCycleFormModel;
	};

	//ExpenseCycleFormModel
	class ExpenseCycleFormModel : public RecordFormModel<ExpenseCycle>
	{
	public:
		static const Field purposeField;
		static const Field positionField;

		ExpenseCycleFormModel(ExpenseCycleView *view, Wt::Dbo::ptr<ExpenseCycle> cyclePtr = Wt::Dbo::ptr<ExpenseCycle>());
		virtual Wt::WWidget *createFormWidget(Wt::WFormModel::Field field) override;
		virtual bool saveChanges() override;

	protected:
		ExpenseCycleView *_view = nullptr;
	};

	//ExpenseCycleView
	class ExpenseCycleView : public EntryCycleView
	{
	public:
		ExpenseCycleView(Wt::Dbo::ptr<ExpenseCycle> cyclePtr = Wt::Dbo::ptr<ExpenseCycle>());
		virtual void init() override;
		virtual Wt::WWidget *createFormWidget(Wt::WFormModel::Field field) override;

		void handlePurposeChanged();
		void handlePositionChanged();
		Wt::WDialog *createAddPositionDialog();

		virtual Wt::WString viewName() const override;
		virtual std::string viewInternalPath() const override { return cyclePtr() ? ExpenseCycle::viewInternalPath(cyclePtr().id()) : ""; }
		virtual RecordFormView *createFormView() override { return new ExpenseCycleView(); }

		Wt::WComboBox *purposeCombo() const { return _purposeCombo; }
		QueryProxyModelCB<PositionProxyModel> *positionCombo() const { return _positionCombo; }

		ExpenseCycleFormModel *model() const { return _model; }
		Wt::Dbo::ptr<ExpenseCycle> cyclePtr() const { return _model->recordPtr(); }

	protected:
		Wt::WComboBox *_purposeCombo = nullptr;
		QueryProxyModelCB<PositionProxyModel> *_positionCombo = nullptr;
		ExpenseCycleFormModel *_model = nullptr;
		Wt::Dbo::ptr<ExpenseCycle> _tempPtr;
	};

	//IncomeCycleFormModel
	class IncomeCycleFormModel : public RecordFormModel<IncomeCycle>
	{
	public:
		static const Field purposeField;
		static const Field serviceField;

		IncomeCycleFormModel(IncomeCycleView *view, Wt::Dbo::ptr<IncomeCycle> cyclePtr = Wt::Dbo::ptr<IncomeCycle>());
		virtual Wt::WWidget *createFormWidget(Wt::WFormModel::Field field) override;
		virtual bool saveChanges() override;

	protected:
		IncomeCycleView *_view = nullptr;
	};

	//IncomeCycleView
	class IncomeCycleView : public EntryCycleView
	{
	public:
		IncomeCycleView(Wt::Dbo::ptr<IncomeCycle> cyclePtr = Wt::Dbo::ptr<IncomeCycle>());
		virtual void init() override;
		virtual Wt::WWidget *createFormWidget(Wt::WFormModel::Field field) override;

		void handlePurposeChanged();
		void handleServiceChanged();
		Wt::WDialog *createAddServiceDialog();

		virtual Wt::WString viewName() const override;
		virtual std::string viewInternalPath() const override { return cyclePtr() ? IncomeCycle::viewInternalPath(cyclePtr().id()) : ""; }
		virtual RecordFormView *createFormView() override { return new IncomeCycleView(); }

		Wt::WComboBox *purposeCombo() const { return _purposeCombo; }
		QueryProxyModelCB<ServiceProxyModel> *serviceCombo() const { return _serviceCombo; }

		IncomeCycleFormModel *model() const { return _model; }
		Wt::Dbo::ptr<IncomeCycle> cyclePtr() const { return _model->recordPtr(); }

	protected:
		Wt::WComboBox *_purposeCombo = nullptr;
		QueryProxyModelCB<ServiceProxyModel> *_serviceCombo = nullptr;
		IncomeCycleFormModel *_model = nullptr;
		Wt::Dbo::ptr<IncomeCycle> _tempPtr;
	};

	//LISTS
	class EntryCycleList : public QueryModelFilteredList<boost::tuple<long long, Wt::WDateTime, std::string, Wt::WDate, Wt::WDate, long long, std::string, CycleInterval, int, long long>>
	{
	public:
		enum ResultColumns { ResId, ResTimestamp, ResEntityName, ResStartDate, ResEndDate, ResAmount, ResExtraName, ResInterval, ResNIntervals, ResEntityId };
		enum ViewColumns { ViewId, ViewCreatedOn, ViewEntity, ViewStartDate, ViewEndDate, ViewAmount, ViewExtra };

		EntryCycleList();
		virtual void load() override;

	protected:
		virtual void initFilters() override;
	};
	class EntityEntryCycleList : public QueryModelFilteredList<boost::tuple<long long, Wt::WDateTime, Wt::WDate, Wt::WDate, long long, std::string, CycleInterval, int>>
	{
	public:
		enum ResultColumns { ResId, ResTimestamp, ResStartDate, ResEndDate, ResAmount, ResExtraName, ResInterval, ResNIntervals };

		EntityEntryCycleList();
		virtual void load() override;

	protected:
		virtual void initFilters() override;
	};

	class IncomeCycleList : public EntryCycleList
	{
	public:
		IncomeCycleList();

	protected:
		virtual void initModel() override;
	};
	class EntityIncomeCycleList : public EntityEntryCycleList
	{
	public:
		EntityIncomeCycleList(Wt::Dbo::ptr<Entity> entityPtr);

	protected:
		virtual void initModel() override;
		Wt::Dbo::ptr<Entity> _entityPtr;
	};

	class ExpenseCycleList : public EntryCycleList
	{
	public:
		ExpenseCycleList();

	protected:
		virtual void initModel() override;
	};
	class EntityExpenseCycleList : public EntityEntryCycleList
	{
	public:
		EntityExpenseCycleList(Wt::Dbo::ptr<Entity> entityPtr);

	protected:
		virtual void initModel() override;
		Wt::Dbo::ptr<Entity> _entityPtr;
	};

	template<class FilteredList>
	class BaseEntryCycleListProxyModel : public Wt::WBatchEditProxyModel
	{
	public:
		BaseEntryCycleListProxyModel(const std::string &pathPrefix, Wt::WAbstractItemModel *model, Wt::WObject *parent = nullptr)
			: Wt::WBatchEditProxyModel(parent), _pathPrefix(pathPrefix)
		{
			setSourceModel(model);
			addAdditionalColumns();
		}
		virtual boost::any data(const Wt::WModelIndex &idx, int role = Wt::DisplayRole) const override;
		virtual boost::any headerData(int section, Wt::Orientation orientation = Wt::Horizontal, int role = Wt::DisplayRole) const override;
		virtual Wt::WFlags<Wt::ItemFlag> flags(const Wt::WModelIndex &index) const override;

	protected:
		void addAdditionalColumns();
		int _linkColumn = -1;
		std::string _pathPrefix;
	};

	template<class FilteredList>
	class EntryCycleListProxyModel : public BaseEntryCycleListProxyModel<FilteredList>
	{
	public:
		EntryCycleListProxyModel(const std::string &pathPrefix, Wt::WAbstractItemModel *model, Wt::WObject *parent = nullptr) : BaseEntryCycleListProxyModel(pathPrefix, model, parent) { }
		virtual boost::any data(const Wt::WModelIndex &idx, int role = Wt::DisplayRole) const override;
	};

	//TEMPLATE CLASS DEFITIONS

// 	class IncomeCycleListProxyModel : public EntryCycleListProxyModel
// 	{
// 	public:
// 		IncomeCycleListProxyModel(Wt::WAbstractItemModel *model, Wt::WObject *parent = nullptr) : EntryCycleListProxyModel(model, parent) { }
// 		virtual boost::any data(const Wt::WModelIndex &idx, int role = Wt::DisplayRole) const override;
// 	};
// 	class ExpenseCycleListProxyModel : public EntryCycleListProxyModel
// 	{
// 	public:
// 		ExpenseCycleListProxyModel(Wt::WAbstractItemModel *model, Wt::WObject *parent = nullptr) : EntryCycleListProxyModel(model, parent) { }
// 		virtual boost::any data(const Wt::WModelIndex &idx, int role = Wt::DisplayRole) const override;
// 	};

}

#endif