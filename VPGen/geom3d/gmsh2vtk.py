import vtk
filename = 'GFG.vtk'
reader = vtk.vtkUnstructuredGridReader()
reader.SetFileName(filename)
reader.Update()
data = reader.GetOutput()

surface_filter = vtk.vtkGeometryFilter()
surface_filter.SetInputConnection(reader.GetOutputPort())
surface_filter.SetPassThroughPointIds(True)

mapper = vtk.vtkDataSetMapper()
mapper.SetInputConnection(surface_filter.GetOutputPort())
actor = vtk.vtkActor()
actor.GetProperty().SetOpacity(0.5)
actor.SetMapper(mapper)

interactor = vtk.vtkRenderWindowInteractor()


renderer = vtk.vtkRenderer()
renderer.SetBackground(1, 1, 1)
renderer.AddActor(actor)
renderer.ResetCamera()
render_window = vtk.vtkRenderWindow()
render_window.AddRenderer(renderer)
interactor.SetRenderWindow(render_window)

interactor.Initialize()
interactor.Start()
